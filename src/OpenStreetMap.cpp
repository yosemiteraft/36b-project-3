#include "OpenStreetMap.h"
#include <unordered_map>

struct COpenStreetMap::SImplementation{
    const std::string DOSMTag = "osm";
    const std::string DNodeTag = "node";
    const std::string DWayTag = "way";

    struct SNode: public CStreetMap::SNode{
        const std::string DNodeIDAttr = "id";
        const std::string DNodeLatAttr = "lat";
        const std::string DNodeLonAttr = "lon";
        const std::string DTagTag = "tag";
        const std::string DTagKeyAttr = "k";
        const std::string DTagValueAttr = "v";

        TNodeID DID;
        SLocation DLocation;
        std::vector<std::pair<std::string, std::string>> DAttributes; // stores key value pairs of <tag> elements

        SNode(const SXMLEntity &entity){
            auto NodeID = std::stoull(entity.AttributeValue(DNodeIDAttr));
            auto NodeLat = std::stod(entity.AttributeValue(DNodeLatAttr));
            auto NodeLon = std::stod(entity.AttributeValue(DNodeLonAttr));
            DID = NodeID;
            DLocation = SLocation{NodeLat,NodeLon};
        }

        ~SNode(){}

        TNodeID ID() const noexcept override{
            return DID;
        }
        
        SLocation Location() const noexcept override{
            return DLocation;
        }
        
        std::size_t AttributeCount() const noexcept override{
            return DAttributes.size();
        }
        
        std::string GetAttributeKey(std::size_t index) const noexcept override{
            // https://medium.com/@ayushmishra8299/how-to-access-elements-of-a-pair-in-c-9e4cace740a4#:~:text=Accessing%20Elements%20of%20a%20Pair,element%20we%20want%20to%20access.
            if(index < DAttributes.size()) {
                return DAttributes[index].first;
            }
            // https://stackoverflow.com/questions/26587110/returning-an-empty-string-efficient-way-in-c
            return {};
        }
        
        bool HasAttribute(const std::string &key) const noexcept override{
            for(auto &attr : DAttributes) {
                if(attr.first == key) {
                    return true;
                }
            }
            return false;
        }
        
        std::string GetAttribute(const std::string &key) const noexcept override{
            for(auto &attr : DAttributes) {
                if(attr.first == key) {
                    return attr.second;
                }
            }
            return {};
        }

        // Function to add key value pair to DAttributes from <tag> element
        void AddAttribute(const std::string &key, const std::string &value) {
            DAttributes.push_back(std::make_pair(key, value));
        }
    };

    struct SWay: public CStreetMap::SWay{
        const std::string DWayIDAttr = "id";
        const std::string DNdTag = "nd";
        const std::string DRefAttr = "ref";
        const std::string DTagTag = "tag";
        const std::string DTagKeyAttr = "k";
        const std::string DTagValueAttr = "v";

        TWayID DID;
        std::vector<std::pair<std::string, std::string>> DAttributes;
        std::vector<TNodeID> DNodeIDs;

        SWay(const SXMLEntity &entity){
            auto WayID = std::stoull(entity.AttributeValue(DWayIDAttr));
            DID = WayID;
        }

        ~SWay(){}

        TWayID ID() const noexcept override{
            return DID;
        }
        
        std::size_t NodeCount() const noexcept override{
            return DNodeIDs.size();
        }
        
        TNodeID GetNodeID(std::size_t index) const noexcept override{
            if(index < DNodeIDs.size()) {
                return DNodeIDs[index];
            }
            return InvalidNodeID;
        }
        
        std::size_t AttributeCount() const noexcept override{
            return DAttributes.size();
        }
        
        std::string GetAttributeKey(std::size_t index) const noexcept override{
            if(index < DAttributes.size()) {
                return DAttributes[index].first;
            }
            return {};
        }
        
        bool HasAttribute(const std::string &key) const noexcept override{
            for(auto &attr : DAttributes) {
                if(attr.first == key) {
                    return true;
                }
            }
            return false;
        }
        
        std::string GetAttribute(const std::string &key) const noexcept override{
            for(auto &attr : DAttributes) {
                if(attr.first == key) {
                    return attr.second;
                }
            }
            return {};
        }

        // Function to add key value pair to DAttributes from <tag> element
        void AddAttribute(const std::string &key, const std::string &value) {
            DAttributes.push_back(std::make_pair(key, value));
        }

        // Function to add node ID to DNodeIDs from <nd> element
        void AddNodeID(TNodeID nodeid) {
            DNodeIDs.push_back(nodeid);
        }
    };

    std::vector<std::shared_ptr<SNode>> DNodesByIndex;
    std::unordered_map<TNodeID,std::shared_ptr<SNode>> DNodesByID;
    std::vector<std::shared_ptr<SWay>> DWaysByIndex;
    std::unordered_map<TWayID,std::shared_ptr<SWay>> DWaysByID;

    bool FindStartTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
        SXMLEntity TempEntity;
        while(xmlsource->ReadEntity(TempEntity,true)){
            if((TempEntity.DType == SXMLEntity::EType::StartElement)&&(TempEntity.DNameData == starttag)){
                return true;
            }
        }
        return false;
    }

    bool FindEndTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &endtag){
        SXMLEntity TempEntity;
        while(xmlsource->ReadEntity(TempEntity,true)){
            if((TempEntity.DType == SXMLEntity::EType::EndElement)&&(TempEntity.DNameData == endtag)){
                return true;
            }
        }
        return false;
    }

    bool ParseOSM(std::shared_ptr<CXMLReader> src){
        SXMLEntity TempEntity;
        if(!FindStartTag(src, DOSMTag)){
            return false;
        }
        while(src->ReadEntity(TempEntity)){
            // if the end of OSM is reached
            if(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == DOSMTag){
                return true;
            } else if(TempEntity.DType == SXMLEntity::EType::StartElement) {
                if(TempEntity.DNameData == DNodeTag) {
                    ParseNode(src, TempEntity);
                } else if(TempEntity.DNameData == DWayTag) {
                    ParseWay(src, TempEntity);
                }
            }
        }
        return true;
    }

    bool ParseNode(std::shared_ptr<CXMLReader> src, SXMLEntity &entity) {
        auto Node = std::make_shared<SNode>(entity);
        SXMLEntity TempEntity;

        while(src->ReadEntity(TempEntity)){
            if(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == DNodeTag) {
                break;
            }
            if((TempEntity.DType == SXMLEntity::EType::StartElement) && (TempEntity.DNameData == Node->DTagTag)) {
                std::string key = TempEntity.AttributeValue(Node->DTagKeyAttr);
                std::string value = TempEntity.AttributeValue(Node->DTagValueAttr);
                Node->AddAttribute(key, value);
            }
        }
        DNodesByIndex.push_back(Node);
        TNodeID id = Node->ID();
        DNodesByID[id] = Node;
        return true;
    }

    bool ParseWay(std::shared_ptr<CXMLReader> src, SXMLEntity &entity) {
        auto Way = std::make_shared<SWay>(entity);
        SXMLEntity TempEntity;

        while(src->ReadEntity(TempEntity)) {
            if(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == DWayTag) {
                break;
            }
            if(TempEntity.DType == SXMLEntity::EType::StartElement) {
                if(TempEntity.DNameData == Way->DNdTag) {
                    TNodeID nodeid = std::stoull(TempEntity.AttributeValue(Way->DRefAttr)); // value of ref attr stored as TNodeID
                    Way->AddNodeID(nodeid);
                } else if(TempEntity.DNameData == Way->DTagTag) {
                    std::string key = TempEntity.AttributeValue(Way->DTagKeyAttr);
                    std::string value = TempEntity.AttributeValue(Way->DTagValueAttr);
                    Way->AddAttribute(key, value);
                }
            }
        }
        DWaysByIndex.push_back(Way);
        TNodeID id = Way->ID();
        DWaysByID[id] = Way;
        return true;
    }

    SImplementation(std::shared_ptr<CXMLReader> src){
        if(src) {
            ParseOSM(src);
        }
    }

    std::size_t NodeCount() const noexcept{
        return DNodesByIndex.size();
    }

    std::size_t WayCount() const noexcept{
        return DWaysByIndex.size();
    }

    std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept{
        if(index < DNodesByIndex.size()){
            return DNodesByIndex[index];
        }
        return nullptr;
    }

    std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept{
        auto Search = DNodesByID.find(id);
        if(Search != DNodesByID.end()){
            return Search->second;
        }
        return nullptr;
    }

    std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept{
        if(index < DWaysByIndex.size()) {
            return DWaysByIndex[index];
        }
        return nullptr;
    }

    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept{
        auto Search = DWaysByID.find(id);
        if(Search != DWaysByID.end()){
            return Search->second;
        }
        return nullptr;
    }
};


COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src){
    DImplementation = std::make_unique<SImplementation>(src);
}

COpenStreetMap::~COpenStreetMap(){}

std::size_t COpenStreetMap::NodeCount() const noexcept{
    return DImplementation->NodeCount();
}

std::size_t COpenStreetMap::WayCount() const noexcept{
    return DImplementation->WayCount();
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept{
    return DImplementation->NodeByIndex(index);
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept{
    return DImplementation->NodeByID(id);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept{
    return DImplementation->WayByIndex(index);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept{
    return DImplementation->WayByID(id);
}

