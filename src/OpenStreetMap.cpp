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
        TNodeID DID;
        SLocation DLocation;
        std::vector<std::pair<std::string, std::string>> DAttributes;

        SNode(const SXMLEntity &entity){
            auto NodeID = std::stoull(entity.AttributeValue(DNodeIDAttr));
            auto NodeLat = std::stod(entity.AttributeValue(DNodeLatAttr));
            auto NodeLon = std::stod(entity.AttributeValue(DNodeLonAttr));
            DID = NodeID;
            DLocation = SLocation{NodeLat,NodeLon};
            DAttributes = entity.DAttributes;
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
        
    };

    struct SWay: public CStreetMap::SWay{
        TWayID DID;
        std::vector<std::pair<std::string, std::string>> DAttributes;
        std::vector<TNodeID> DNodeIDs;

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
    };

    std::vector<std::shared_ptr<SNode>> DNodesByIndex;
    std::unordered_map<TNodeID,std::shared_ptr<SNode>> DNodesByID;

    bool FindStartTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
        SXMLEntity TempEntity;
        while(xmlsource->ReadEntity(TempEntity,true)){
            if((TempEntity.DType == SXMLEntity::EType::StartElement)&&(TempEntity.DNameData == starttag)){
                return true;
            }
        }
        return false;
    }

    bool FindEndTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
        SXMLEntity TempEntity;
        while(xmlsource->ReadEntity(TempEntity,true)){
            if((TempEntity.DType == SXMLEntity::EType::EndElement)&&(TempEntity.DNameData == starttag)){
                return true;
            }
        }
        return false;
    }

    bool ParseOSM(std::shared_ptr<CXMLReader> src){
        SXMLEntity TempEntity;
        if(!FindStartTag(src,DOSMTag)){
            return false;
        }
        while(src->ReadEntity(TempEntity)){
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DNodeTag){
                auto NewNode = std::make_shared<SNode>(TempEntity);
                DNodesByIndex.push_back(NewNode);
                DNodesByID[NewNode->ID()] = NewNode;
                FindEndTag(src,DNodeTag);
            }
        }


    }

    SImplementation(std::shared_ptr<CXMLReader> src){

    }

    std::size_t NodeCount() const noexcept{
        return DNodesByIndex.size();
    }

    std::size_t WayCount() const noexcept{
        return 0;
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
        
    }

    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept{
        
    }

};


COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src){
    DImplementation = std::make_unique<SImplementation>(src);
}

COpenStreetMap::~COpenStreetMap(){

}

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

