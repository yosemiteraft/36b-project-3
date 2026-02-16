#include "XMLBusSystem.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>
using std::cout;
using std::endl;

struct CXMLBusSystem::SImplementation{
    const std::string DBusSystemTag = "bussystem";
    const std::string DStopsTag = "stops";
    const std::string DStopTag = "stop";
    const std::string DRoutesTag = "routes";
    const std::string DRouteTag = "route";
    const std::string DStopIDAttr = "id";
    const std::string DStopNodeAttr = "node";
    const std::string DStopDescAttr = "description";
    const std::string DRouteNameAttr = "name";
    const std::string DRouteStopTag = "routestop";
    const std::string DRouteStopAttr = "stop";

    const std::string DPathSourceAttr = "source";
    const std::string DPathDestAttr = "destination";
    const std::string DPathTag = "path";
    const std::string DPathsTag = "paths";
    const std::string DNodeTag = "node";
    const std::string DNodeIDAttr = "id";

    struct SStop : public CBusSystem::SStop {
        TStopID DID;
        CStreetMap::TNodeID DNodeID;
        std::string DDescription;

        SStop(TStopID id, CStreetMap::TNodeID nodeid, const std::string &description) {
            DID = id;
            DNodeID = nodeid;
            DDescription = description;
        }
        ~SStop(){};

        TStopID ID() const noexcept override {
            return DID;
        }

        CStreetMap::TNodeID NodeID() const noexcept override {
            return DNodeID;
        }

        std::string Description() const noexcept override {
            return DDescription;
        }

        std::string Description(const std::string &description) noexcept override {
            DDescription = description;
            return DDescription;
        }
    };

    struct SRoute : public CBusSystem::SRoute {
        std::string DName;
        std::vector<TStopID> DStopIDs;

        SRoute(const std::string name) {
            DName = name;
        }
        ~SRoute(){};

        std::string Name() const noexcept override {
            return DName;
        }

        std::size_t StopCount() const noexcept override {
            return DStopIDs.size();
        }

        TStopID GetStopID(std::size_t index) const noexcept override {
            // https://www.w3schools.com/cpp/cpp_vectors.asp
            return DStopIDs[index];
        }

        void AddStop(TStopID stopid) {
            DStopIDs.push_back(stopid);
        }
    };

    struct SPath : public CBusSystem::SPath {
        CStreetMap::TNodeID DStartNodeID;
        CStreetMap::TNodeID DEndNodeID;
        std::vector<CStreetMap::TNodeID> DNodeIDs;

        SPath(CStreetMap::TNodeID startnodeid, CStreetMap::TNodeID endnodeid) {
            DStartNodeID = startnodeid;
            DEndNodeID = endnodeid;
        }
        ~SPath(){};

        CStreetMap::TNodeID StartNodeID() const noexcept override {
            return DStartNodeID;
        }

        CStreetMap::TNodeID EndNodeID() const noexcept override {
            return DEndNodeID;
        }

        std::size_t NodeCount() const noexcept override {
            return DNodeIDs.size();
        }

        CStreetMap::TNodeID GetNodeID(std::size_t index) const noexcept override {
            return DNodeIDs[index];
        }

        void AddNode(CStreetMap::TNodeID nodeid) {
            DNodeIDs.push_back(nodeid);
        }
    };

    bool FindStartTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
        SXMLEntity TempEntity;
        while(xmlsource->ReadEntity(TempEntity, true)){
            if((TempEntity.DType == SXMLEntity::EType::StartElement)&&(TempEntity.DNameData == starttag)){
                return true;
            }
        }
        return false;
    }

    bool FindEndTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
        SXMLEntity TempEntity;
        while(xmlsource->ReadEntity(TempEntity, true)){
            if((TempEntity.DType == SXMLEntity::EType::EndElement)&&(TempEntity.DNameData == starttag)){
                return true;
            }
        }
        return false;
    }

    std::vector<std::shared_ptr<SStop>> DStopsByIndex;
    std::unordered_map<TStopID, std::shared_ptr<SStop>> DStopsByID;

    std::vector<std::shared_ptr<SRoute>> DRoutesByIndex;
    std::unordered_map<std::string, std::shared_ptr<SRoute>> DRoutesByName;

    // https://medium.com/@gulshansharma014/call-to-implicitly-deleted-default-constructor-of-unordered-map-pair-int-int-int-d3b2a6da0b41
    std::map<std::pair<CStreetMap::TNodeID, CStreetMap::TNodeID>, std::shared_ptr<SPath>> DPathsByNodes;


    void ParseStop(std::shared_ptr< CXMLReader > systemsource, const SXMLEntity &stop){
        TStopID StopID = std::stoull(stop.AttributeValue(DStopIDAttr)); // string to unsigned long long
        CStreetMap::TNodeID NodeID = std::stoull(stop.AttributeValue(DStopNodeAttr));
        auto NewStop = std::make_shared<SStop>(StopID, NodeID, stop.AttributeValue(DStopDescAttr));
        DStopsByIndex.push_back(NewStop);
        DStopsByID[StopID] = NewStop;
        FindEndTag(systemsource, DStopTag);
    }

    void ParseStops(std::shared_ptr< CXMLReader > systemsource){
        SXMLEntity TempEntity;

        do{
            if(!systemsource->ReadEntity(TempEntity, true)) {
                return;
            }
            if((TempEntity.DType == SXMLEntity::EType::StartElement) && (TempEntity.DNameData == DStopTag)) {
                ParseStop(systemsource,TempEntity);
            }

        }while((TempEntity.DType != SXMLEntity::EType::EndElement)||(TempEntity.DNameData != DStopsTag));
    }

    void ParseRoute(std::shared_ptr< CXMLReader > systemsource, const SXMLEntity &route){
        std::string RouteName = route.AttributeValue(DRouteNameAttr);
        auto NewRoute = std::make_shared<SRoute>(RouteName);
        SXMLEntity TempEntity; // reads in routestops
        
        do {
            if(!systemsource->ReadEntity(TempEntity, true)) {
                return;
            }
            if((TempEntity.DType == SXMLEntity::EType::StartElement) && (TempEntity.DNameData == DRouteStopTag)) {
                TStopID StopID = std::stoull(TempEntity.AttributeValue(DRouteStopAttr));
                NewRoute->AddStop(StopID);
            }
        } while((TempEntity.DType !=  SXMLEntity::EType::EndElement) || (TempEntity.DNameData != DRouteTag)); // loop until </route> is reached

        DRoutesByIndex.push_back(NewRoute);
        DRoutesByName[RouteName] = NewRoute;
    }

    void ParseRoutes(std::shared_ptr< CXMLReader > systemsource){
        SXMLEntity TempEntity;

        do{
            if(!systemsource->ReadEntity(TempEntity, true)){
                return;
            }
            if((TempEntity.DType == SXMLEntity::EType::StartElement) && (TempEntity.DNameData == DRouteTag)){
                ParseRoute(systemsource, TempEntity);
            }
        } while((TempEntity.DType != SXMLEntity::EType::EndElement) || (TempEntity.DNameData != DRoutesTag));
    }
    
    void ParsePath(std::shared_ptr< CXMLReader > systemsource, const SXMLEntity &path) {
        CStreetMap::TNodeID startnode = std::stoull(path.AttributeValue(DPathSourceAttr));
        CStreetMap::TNodeID endnode = std::stoull(path.AttributeValue(DPathDestAttr));
        auto NewPath = std::make_shared<SPath>(startnode, endnode);
        SXMLEntity TempEntity;

        do {
            if(!systemsource->ReadEntity(TempEntity, true)) {
                return;
            }
            if((TempEntity.DType == SXMLEntity::EType::StartElement) && (TempEntity.DNameData == DNodeTag)) {
                CStreetMap::TNodeID NodeID = std::stoull(TempEntity.AttributeValue(DNodeIDAttr));
                NewPath->AddNode(NodeID);
            }
        } while((TempEntity.DType != SXMLEntity::EType::EndElement) || (TempEntity.DNameData != DPathTag));
        
        std::pair<CStreetMap::TNodeID, CStreetMap::TNodeID> key = std::make_pair(startnode, endnode);
        DPathsByNodes[key] = NewPath;
    }

    void ParsePaths(std::shared_ptr< CXMLReader > systemsource) {
        SXMLEntity TempEntity;

        do {
            if(!systemsource->ReadEntity(TempEntity, true)) {
                return;
            }
            if((TempEntity.DType == SXMLEntity::EType::StartElement) && (TempEntity.DNameData == DPathTag)) {
                ParsePath(systemsource, TempEntity);
            }
        } while((TempEntity.DType != SXMLEntity::EType::EndElement) || (TempEntity.DNameData != DPathsTag));
    }

    void ParseBusSystem(std::shared_ptr< CXMLReader > systemsource){
        SXMLEntity TempEntity;
        if(!FindStartTag(systemsource, DBusSystemTag)){
            return;
        }
        if(!FindStartTag(systemsource, DStopsTag)){
            return;
        }
        if(!FindStartTag(systemsource, DRoutesTag)) {
            return;
        }
        // call appropriate parsing fns depending on whether it is stops or routes
        if(TempEntity.DType == SXMLEntity::EType::StartElement) {
            if(TempEntity.DNameData == DStopsTag) {
                ParseStops(systemsource);
            } else if(TempEntity.DNameData == DRoutesTag) {
                ParseRoutes(systemsource);
            }
        }
    }

    void ParsePathSystem(std::shared_ptr< CXMLReader > systemsource) {
        if(!FindStartTag(systemsource, DPathsTag)) {
            return;
        }
        ParsePaths(systemsource);
    }

    SImplementation(std::shared_ptr< CXMLReader > systemsource, std::shared_ptr< CXMLReader > pathsource){
        if(systemsource) {
            ParseBusSystem(systemsource);
        }
        if(pathsource) {
            ParsePathSystem(pathsource);
        }
    }

    std::size_t StopCount() const noexcept{
        return DStopsByIndex.size();
    }

    std::size_t RouteCount() const noexcept{
        return DRoutesByIndex.size();
    }
    
    std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept{
        if(index < DStopsByIndex.size()) {
            return DStopsByIndex[index];
        }
        return nullptr;
    }
    
    std::shared_ptr<SStop> StopByID(TStopID id) const noexcept{
        // https://en.cppreference.com/w/cpp/container/unordered_map.html
        // https://en.cppreference.com/w/cpp/container/unordered_map/find.html
        auto target = DStopsByID.find(id);
        if(target != DStopsByID.end()) {
            return target->second;
        }
        return nullptr;
    }
    
    std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept{
        if(index < DRoutesByIndex.size()) {
            return DRoutesByIndex[index];
        }
        return nullptr;
    }
    
    std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept{
        auto target = DRoutesByName.find(name);
        if(target != DRoutesByName.end()) {
            return target->second;
        }
        return nullptr;
    }
    
    std::shared_ptr<SPath> PathByStopIDs(TStopID start, TStopID end) const noexcept{
        std::shared_ptr<SStop> startstop = StopByID(start);
        std::shared_ptr<SStop> endstop = StopByID(end);
        if(!startstop || !endstop) {
            return nullptr;
        }

        CStreetMap::TNodeID startnode = startstop->NodeID();
        CStreetMap::TNodeID endnode = endstop->NodeID();

        // https://cplusplus.com/reference/utility/pair/pair/
        // https://cplusplus.com/reference/utility/make_pair/
        std::pair<CStreetMap::TNodeID, CStreetMap::TNodeID> key = std::make_pair(startnode, endnode);
        auto target = DPathsByNodes.find(key);
        if(target != DPathsByNodes.end()) {
            return target->second;
        }
        return nullptr;
    }
    
};

CXMLBusSystem::CXMLBusSystem(std::shared_ptr< CXMLReader > systemsource, std::shared_ptr< CXMLReader > pathsource){
    DImplementation = std::make_unique<SImplementation>(systemsource,pathsource);
}
    
CXMLBusSystem::~CXMLBusSystem(){}
    
std::size_t CXMLBusSystem::StopCount() const noexcept{
    return DImplementation->StopCount();
}
    
std::size_t CXMLBusSystem::RouteCount() const noexcept{
    return DImplementation->RouteCount();
}

std::shared_ptr<CBusSystem::SStop> CXMLBusSystem::StopByIndex(std::size_t index) const noexcept{
    return DImplementation->StopByIndex(index);
}

std::shared_ptr<CBusSystem::SStop> CXMLBusSystem::StopByID(TStopID id) const noexcept{
    return DImplementation->StopByID(id);
}

std::shared_ptr<CBusSystem::SRoute> CXMLBusSystem::RouteByIndex(std::size_t index) const noexcept{
    return DImplementation->RouteByIndex(index);
}

std::shared_ptr<CBusSystem::SRoute> CXMLBusSystem::RouteByName(const std::string &name) const noexcept{
    return DImplementation->RouteByName(name);
}

std::shared_ptr<CBusSystem::SPath> CXMLBusSystem::PathByStopIDs(TStopID start, TStopID end) const noexcept{
    return DImplementation->PathByStopIDs(start, end);
}
