
#include "../test.h"
#include "wjson/json.hpp"

using namespace wjson;

static void GenStat(Stat& stat, const ValueWarpConst& v) {
	switch (v.GetType()) {
		case wjson::ValueType::Array:
			for(auto it = v.ArrayBegin();it != v.ArrayEnd();++it) GenStat(stat,*it);
			stat.arrayCount++;
			stat.elementCount += v.Size();
			break;
		
		case wjson::ValueType::Object:
			for(auto it = v.ObjectBegin();it != v.ObjectEnd();++it) {
				GenStat(stat,it.Value());
				stat.stringLength += it.Key().size();
			}
			stat.objectCount++;
			stat.memberCount += v.Size();
			stat.stringCount += v.Size();
			break;

		case wjson::ValueType::Number:
			stat.numberCount++;
			break;

		case wjson::ValueType::Bool:
			if (v.GetBool())
				stat.trueCount++;
			else
				stat.falseCount++;
			break;

		case wjson::ValueType::Null:
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
