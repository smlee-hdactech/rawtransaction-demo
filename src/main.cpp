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
};

PubNPrivate obtainKeyPairs()
{
    KeysHelperWithRpc helper(RpcClient("13.125.145.98", 4260, "hdacrpc", "1234", "kcc"));
    KeyPairs result;
    const string storeFileName("key_store.dat");
    ifstream keyStore(storeFileName);
    if (!keyStore.is_open()) {
        ofstream keyStore(storeFileName);
        KeyPairs pairs = createKeyPairs(helper.privHelper(), helper.addrHelper());
        Object pairsValue;
        pairsValue.push_back(Pair("private", pairs.privateKey));
        pairsValue.push_back(Pair("public", pairs.pubkey));
        write(pairsValue, keyStore);
    }

    //read_stream()
    return PubNPrivate();
}

int main()
{
    cout << "first" << endl;
    return 0;
}
