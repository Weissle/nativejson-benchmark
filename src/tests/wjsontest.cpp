
#include "../test.h"
#include "wjson/json.hpp"
#include <sstream>


using namespace wjson;

static void GenStat(Stat& stat, const ValueWarpConst& v) {
	switch (v.GetType()) {
		case ValueType::Array:
			for(auto it = v.ArrayBegin();it != v.ArrayEnd();++it) GenStat(stat,*it);
			stat.arrayCount++;
			stat.elementCount += v.Size();
			break;
		
		case ValueType::Object:
			for(auto it = v.ObjectBegin();it != v.ObjectEnd();++it) {
				GenStat(stat,it.Value());
				stat.stringLength += it.Key().size();
			}
			stat.objectCount++;
			stat.memberCount += v.Size();
			stat.stringCount += v.Size();
			break;

		case ValueType::String:
			stat.stringCount++;
			stat.stringLength += v.Size();
			break;

		case ValueType::Number:
			stat.numberCount++;
			break;

		case ValueType::Bool:
			if (v.GetBool())
				stat.trueCount++;
			else
				stat.falseCount++;
			break;

		case ValueType::Null:
			stat.nullCount++;
			break;
	}

}

class WjsonParseResult : public ParseResultBase {
public:
    Json root;
};

class WjsonStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }

    std::string s;
};

class WjsonTest : public TestBase {
public:
#if TEST_INFO
    virtual const char* GetName() const { return "wjson"; }
    virtual const char* GetFilename() const { return __FILE__; }
#endif

#if TEST_PARSE
    virtual ParseResultBase* Parse(const char* j, size_t length) const {
        (void)length;
        WjsonParseResult* pr = new WjsonParseResult;
        try {
            //pr->root = json::parse(j);
			pr->root.Parse(j);
        }
        catch (...) {
            delete pr;
            return 0;
        }
    	return pr;
    }
#endif

#if TEST_STRINGIFY
    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const {
        const WjsonParseResult* pr = static_cast<const WjsonParseResult*>(parseResult);
        WjsonStringResult* sr = new WjsonStringResult;

        //sr->s = pr->root.dump();
		std::stringstream stream;
		pr->root.Dump(stream,0);
		sr->s = stream.str();

        return sr;
    }
#endif

#if TEST_PRETTIFY
    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const {
        const WjsonParseResult* pr = static_cast<const WjsonParseResult*>(parseResult);
        WjsonStringResult* sr = new WjsonStringResult;

        //sr->s = pr->root.dump(4);
		std::stringstream stream;
		pr->root.Dump(stream,4,' ');
		sr->s = stream.str();
        return sr;
    }
#endif

#if TEST_STATISTICS
    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const {
        const WjsonParseResult* pr = static_cast<const WjsonParseResult*>(parseResult);
        memset(stat, 0, sizeof(Stat));
        GenStat(*stat, pr->root);
        return true;
    }
#endif

#if TEST_CONFORMANCE
    virtual bool ParseDouble(const char* j, double* d) const {
        try {
            //json root = json::parse(j);
            //*d = root[0].get<double>();
			Json json;
			json.ParseAny(j);
			*d = json.GetDouble();
            return true;
        }
        catch (...) {
        }
        return false;
    }

    virtual bool ParseString(const char* j, std::string& s) const {
        try {
            //json root = json::parse(j);
            //s = root[0].get<std::string>();
            Json json;
			json.ParseAny(j);
			s = json.GetString();
            return true;
        }
        catch (...) {
        }
        return false;
    }
#endif

REGISTER_TEST(WjsonTest);

};
