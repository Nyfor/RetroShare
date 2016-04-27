#pragma once

#include "ApiTypes.h"
#include "json.h"

namespace resource_api
{

class JsonStream: public StreamBase
{
public:
    JsonStream();
    virtual ~JsonStream();

    void setJsonString(std::string jsonStr);
    std::string getJsonString();

    // it is possible to use this class as buffer
    // first use as serialiser and fill with values
    // then call this method to deserialise the values
    void switchToDeserialisation();


    //----------Stream Interface ---------------

    // make an array
    StreamBase& operator<<(ValueReference<bool> value) override;
    StreamBase& operator<<(ValueReference<int> value) override;
    StreamBase& operator<<(ValueReference<double> value) override;
    StreamBase& operator<<(ValueReference<std::string> value) override;
    // usefull if the new array member should be an array or object
    // the reference should be at least valid until another method of this class gets called
    StreamBase& getStreamToMember() override;

    // make an object
    StreamBase& operator<<(KeyValueReference<bool> keyValue) override;
    StreamBase& operator<<(KeyValueReference<int> keyValue) override;
    StreamBase& operator<<(KeyValueReference<double> keyValue) override;
    StreamBase& operator<<(KeyValueReference<std::string> keyValue) override;
    // usefull if the new object member should be an array or object
    // the reference should be at least valid until another method of this class gets called
    StreamBase& getStreamToMember(std::string name) override;

    // make a binay data object (not a real object, just binary data)
    // idea: can use vector.swap() to allow passing larger data items without copying
    StreamBase& operator<<(std::vector<uint8_t>& data) override;

    // return true if there are more members in this object/array
    // useful for array reading
    bool hasMore() override;

    bool serialise() override; // let external operators find out they should serialise or deserialise
    // return true if no serialisation/deserialisation error occoured
    bool isOK() override;
    void setError() override; // let external operators set the failed bit
    //virtual void addLogMsg(std::string msg); // mayb remove? (put log messages to error log einstead)
    void addErrorMsg(std::string msg) override;
    std::string getLog() override;
    std::string getErrorLog() override;

    bool isRawData() override;
    std::string getRawData() override;
private:
    bool mSerialise;
    enum DataType{ TYPE_UNDEFINED, TYPE_ARRAY, TYPE_OBJECT, TYPE_RAW };
    // check if the current type is undefined
    // if not check if the new type matches the old type
    // if not set the error bit
    void setType(DataType type);
    DataType mDataType;

    json::Value mValue;

    json::Object mObject;
    // check if we are and object
    // check if this key exists
    bool checkObjectMember(std::string key);
    json::Array mArray;
    size_t mArrayNextRead;
    // check if we are an array
    // check if next read is valid
    // if not set error bit
    bool arrayBoundsOk();
    std::string mRawString;

    bool mIsOk;
    std::string mErrorLog;

    // try serialisation and set error bit on error
    bool checkDeserialisation();

    // check if value has correct type
    // if yes return the extracted value
    // if not then set the error bit
    void valueToBool(json::Value& value, bool& boolean);
    void valueToInt(json::Value& value, int& integer);
    void valueToDouble(json::Value& value, double& doubleVal);
    void valueToString(json::Value& value, std::string& str);

    void deleteCurrentChild();
    json::Value getJsonValue();
    JsonStream* mChild;
    std::string mChildKey;
};

} // namespace resource_api
