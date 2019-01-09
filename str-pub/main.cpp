#include <iostream>
#include <keys/hs_keys.h>
#include <json_spirit/json_spirit_reader_template.h>
#include <json_spirit/json_spirit_stream_reader.h>
#include <json_spirit/json_spirit_reader.h>
#include <json_spirit/json_spirit_writer.h>
#include <fstream>
#include <helpers/hs_helpers.h>
#include <rpc/hs_rpc.h>
#include <rpc/rpcclient.h>

using namespace std;
using namespace json_spirit;

struct PubNPrivate {
    string privateKey;
    string publicKey;
    string address;
};

PubNPrivate obtainKeyPairs(const KeysHelperWithRpc & keyHelper)
{
    KeyPairs result;
    PubNPrivate retVal;
    const string storeFileName("key_store.dat");
    ifstream keyStore(storeFileName);
    if (!keyStore.is_open()) {
        ofstream keyStoreW(storeFileName);
        result = createKeyPairs(keyHelper.privHelper(), keyHelper.addrHelper());
        retVal = {result.privateKey, result.pubkey, result.walletAddr};
        Object pairsValue;
        pairsValue.push_back(Pair("private", result.privateKey));
        pairsValue.push_back(Pair("public", result.pubkey));
        pairsValue.push_back(Pair("address", result.walletAddr));
        write(pairsValue, keyStoreW);
        ostringstream ostr;
        ostr << "send authority may be necessary for " << result.walletAddr << ", grant " << result.walletAddr << " send";
        throw ostr.str();
    } else {
        Value pairsValue;
        read(keyStore, pairsValue);
        retVal.privateKey = find_value(pairsValue.get_obj(), "private").get_str();
        retVal.publicKey = find_value(pairsValue.get_obj(), "public").get_str();
        retVal.address = find_value(pairsValue.get_obj(), "address").get_str();
    }

    return retVal;
}

void importAddress(const RpcClient& client, const string& address)
{
    string resultStr;
    if (!rpcResult(importaddress(client, address), resultStr)) {
        cerr << "error on importaddress" << endl;
        return;
    }
    cout << "success import address, " << address << endl;
}

struct UnspentInfo
{
    string txid;
    int vout;
    string scriptPubkey;
};

UnspentInfo chooseUnspent(const RpcClient& client, const string& address)
{
    string resultStr;
    if (!rpcResult(listunspent(client, address), resultStr)) {
        throw "error on importaddress";
    }

    Value resultJson;
    read_string(resultStr, resultJson);
    if (resultJson.type() != array_type) {
        throw "wrong result format";
    }
    //string errorMsg("const msg");
    Array unspents = resultJson.get_array();
    if (unspents.empty())   {
        stringstream ostr;
        ostr << "unspent is necessary for " << address << ", example: grant " << address << " send";
        throw ostr.str();
        //throw errorMsg;
    }
    // TODO : Does asset-existence matter?
    string txid = find_value(unspents[0].get_obj(), "txid").get_str();
    int vout = find_value(unspents[0].get_obj(), "vout").get_int();
    string scriptPubkey = find_value(unspents[0].get_obj(), "scriptPubKey").get_str();

    return {txid, vout, scriptPubkey};
}

string obtainCreateTxid(const RpcClient& client, const string& streamName)
{
    string resultStr;
    if (!rpcResult(liststreams(client, streamName), resultStr)) {
        throw "error on importaddress";
    }

    Value resultJson;
    read_string(resultStr, resultJson);
    if (resultJson.type() != array_type) {
        throw "wrong result format";
    }
    Array streams = resultJson.get_array();
    return find_value(streams[0].get_obj(), "createtxid").get_str();
}

string sendRawTransaction(const RpcClient& client, const string& rawTx)
{
    string resultStr;
    if (!rpcResult(sendrawtx(client, rawTx), resultStr)) {
        throw "error on importaddress";
    }

    return resultStr;
}

int main(int argc, char* argv[])
{
    try {
    RpcClient client("13.125.145.98", 4260, "hdacrpc", "1234", "kcc");
    KeysHelperWithRpc helper(client);

    // 1. parameter로부터 stream 이름, key 이름, item 값을 구한다.
    string streamName(argv[1]);
    string keyName(argv[2]);
    string itemValue(argv[3]);

    // 2. 프라이빗 키와 퍼블릭 키를 생성하거나 로컬에 저장된 값을 얻어온다.
    auto keyPairs = obtainKeyPairs(helper);
    cout << "privateKey : " << keyPairs.privateKey << endl;
    cout << "pubilcKey : " << keyPairs.publicKey << endl;
    cout << "address : " << keyPairs.address << endl;

    // 3. 지갑주소에 대한 권한이 지정되어 있어야 한다. 즉, admin node에서, grant ${address} send

    // 4. 지갑주소에 대해서 import가 되어 있어야 한다.
    importAddress(client, keyPairs.address);

    // 5. address에 대한 unspent tx구하고, txid, vout, sciptPubKey를 추출해낸다.
    auto unspent = chooseUnspent(client, keyPairs.address);
    cout << "unspent txid: " << unspent.txid << endl;
    cout << "unspent vout: " << unspent.vout << endl;
    cout << "unspent scritpPubkey: " << unspent.scriptPubkey << endl;

    // 6. stream의 create txid를 구한다.
    auto createTxid = obtainCreateTxid(client, streamName);
    cout << "create txid: " << createTxid << endl;
    
    // 7. 추출한 정보로부터 스트림을 발행하기 위한 raw-tx를 생성한다.
    string rawTxHex = createStreamPublishTx(
        keyName,
        itemValue,
        createTxid,
        unspent.scriptPubkey,
        unspent.txid,
        unspent.vout,
        "",
        keyPairs.privateKey,
        helper.privHelper()
    );
    cout << "raw-tx hex: " << rawTxHex << endl;

    // 8. 생성한 raw-tx를 전송한다.
    string txid = sendRawTransaction(client, rawTxHex);
    cout << "the id of tx sent: " << txid << endl;
    } catch(exception &e)   {
        cerr << e.what() << endl;
        return -1;
    } catch(string &e) {
        cerr << e << endl;
        return -2;
    }

    return 0;
}
