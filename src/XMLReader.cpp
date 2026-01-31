#include "XMLReader.h"
#include <expat.h>
#include <queue>

// https://libexpat.github.io/doc/getting-started/
struct CXMLReader::SImplementation{
    // https://ftp.ccp4.ac.uk/ccp4/7.0/unpacked/checkout/wxPython-src-3.0.2.0/src/expat/doc/reference.html#XML_ParserCreate
    XML_Parser DParser;
    std::shared_ptr<CDataSource> DSource; // where XML data is coming from
    // https://en.cppreference.com/w/cpp/container/queue.html
    std::queue<SXMLEntity> DQueue; // where parsed entities are stored

    static void ExpatStartElement(void *data, const XML_Char *name, const XML_Char **attrs){
        SImplementation *Implementation = (SImplementation*) data;
        SXMLEntity entity; 
        entity.DType = SXMLEntity::EType::StartElement;
        entity.DNameData = name;
        // https://en.cppreference.com/w/cpp/container/deque/push_back.html
        for(int i=0; attrs[i] != nullptr; i+=2) { // loops through the attributes array
            const char* key = attrs[i];
            const char* value = attrs[i+1];
            entity.DAttributes.push_back({key, value}); // Add key/value pair to the queue
        }
        // https://en.cppreference.com/w/cpp/container/queue.html
        Implementation->DQueue.push(entity);
    }

    static void ExpatEndElement(void *data, const XML_Char *name){
        SImplementation *Implementation = (SImplementation*) data;
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::EndElement;
        entity.DNameData = name;
        Implementation->DQueue.push(entity);
    }

    static void ExpatCharacterData(void *data, const XML_Char *s, int len){
        SImplementation *Implementation = (SImplementation*) data;
        std::string datastr(s, len);
        // https://www.geeksforgeeks.org/cpp/vector-back-in-cpp-stl/
        if(!Implementation->DQueue.empty() && Implementation->DQueue.back().DType == SXMLEntity::EType::CharData) {
            Implementation->DQueue.back().DNameData += datastr;
        } else {
            SXMLEntity entity;
            entity.DType = SXMLEntity::EType::CharData;
            // https://cplusplus.com/reference/string/string/string/
            entity.DNameData = std::string(s, len);
            Implementation->DQueue.push(entity);
        }
    }

    SImplementation(std::shared_ptr< CDataSource > src){
        DSource = src;
        DParser = XML_ParserCreate(nullptr); // default encoding
        // https://svn.apache.org/repos/asf/apr/apr-util/vendor/expat/1.95.2/doc/reference.html
        XML_SetUserData(DParser, this); // so callback functions can access queue, src, etc
        XML_SetElementHandler(DParser, ExpatStartElement, ExpatEndElement);
        XML_SetCharacterDataHandler(DParser, ExpatCharacterData);
    }

    ~SImplementation(){
        // https://svn.apache.org/repos/asf/apr/apr-util/vendor/expat/1.95.2/doc/reference.html
        if(DParser) {
            XML_ParserFree(DParser);
        }
    }

    // Returns true if all entities have been read from the XML
    bool End() const{
        // https://www.geeksforgeeks.org/cpp/queueempty-queuesize-c-stl/
        return DSource->End() && DQueue.empty(); // should have reach end of data source
    }

    // Returns true if the entity is successfully read if skipcdata
    // is true only element type entities will be returned
    bool ReadEntity(SXMLEntity &entity, bool skipcdata){
        if(DQueue.empty() && !DSource->End()){
            RefillQueue();
        }

        while(!DQueue.empty()){
            SXMLEntity currentEntity = DQueue.front();
            DQueue.pop();
            if(skipcdata && currentEntity.DType == SXMLEntity::EType::CharData){
                continue;
            }
            entity = currentEntity;
            return true;
        }
        return false; // Otherwise if the queue is empty, return false
    }
    private:
    void RefillQueue() { //helper function to refill DQueue if data source is not empty
        while (!DSource->End()) {
            std::vector<char> buffer(512);
            size_t numRead = 0;

            while (numRead < buffer.size() && !DSource->End()) {
                char ch;
                if (DSource->Get(ch)) {
                    buffer[numRead] = ch;
                    numRead += 1;
                }
            }

            if (numRead > 0) {
                XML_Parse(DParser, buffer.data(), numRead, DSource->End());
            }
        }
    }

};

CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src){
    DImplementation = std::make_unique<SImplementation>(src);
}

CXMLReader::~CXMLReader(){
    
}

bool CXMLReader::End() const{
    return DImplementation->End();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
    return DImplementation->ReadEntity(entity, skipcdata);
}
