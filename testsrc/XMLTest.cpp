#include <gtest/gtest.h>
#include "XMLReader.h"
#include "StringDataSource.h"

TEST(XMLReaderTest, SimpleTest){
    /* Tests for:
    * - Correct parsing of a basic XML tag with attribute
    * - Valid detection of StartElement and EndElement entity types
    * - Checks attribute existence and correct value retrieval
    * - End of XML detection (empty queue, no more in data source)
    */
    // https://www.w3schools.com/xml/xml_syntax.asp
    std::shared_ptr<CStringDataSource> src = std::make_shared<CStringDataSource>("<tag attr1=\"val1\"></tag>");
    CXMLReader reader(src);
    SXMLEntity entity;
    
    // Test reading of start element
    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "tag");

    // Test reading of attr
    EXPECT_EQ(entity.DAttributes.size(), 1);
    EXPECT_TRUE(entity.AttributeExists("attr1"));
    EXPECT_EQ(entity.AttributeValue("attr1"), "val1");

    // Test reading of end element
    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "tag");

    EXPECT_TRUE(reader.End()); // No more source data and queue should be empty

}

TEST(XMLReaderTest, ElementTest){
    /* Tests for:
    * - Correct parsing of mutliple XML elements (tags within tags)
    * - Correct order of start/end tags (retains order read in)
    * - Detection of nested start and end dlement entities
    * - End of XML detection with nested xml elements
    */
    std::shared_ptr<CStringDataSource> src = std::make_shared<CStringDataSource>("<tag1><tag2></tag2></tag1>");
    CXMLReader reader(src);
    SXMLEntity entity;

    // Test reading of outer start element
    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "tag1");

    // Test reading of inner start element
    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "tag2");

    // Test reading of inner end element
    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "tag2");

    // Test reading of outer end element
    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "tag1");

    EXPECT_TRUE(reader.End());

}

TEST(XMLReaderTest, CDataTest){
    /* Tests for:
    * - Correctly read in CharData (text) entities when skipcdata=false
    * - Skipping of CharData entities when skipcdata=true
    * - End of XML detection with character data
    */
    // Tests for non skipped Cdata
    std::shared_ptr<CStringDataSource> src = std::make_shared<CStringDataSource>("<tag>CData</tag>");
    CXMLReader reader(src);
    SXMLEntity entity;

    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "tag");

    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "CData");

    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "tag");

    EXPECT_TRUE(reader.End());

    // Tests for skipping CData
    std::shared_ptr<CStringDataSource> skipsrc = std::make_shared<CStringDataSource>("<tag>CData</tag>");
    CXMLReader skipreader(skipsrc);

    EXPECT_TRUE(skipreader.ReadEntity(entity, true));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "tag");

    EXPECT_TRUE(skipreader.ReadEntity(entity, true));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "tag");

    EXPECT_TRUE(skipreader.End());

}

TEST(XMLReaderTest, LongCDataTest){
    /* Tests for:
    * - Correct parsing of long CharData (300 characters) in a buffer
    * - All text data read when reading long character strings (not cut off)
    * - Length validation of long CharData entities
    * - Proper end of XML detection for XML with long text
    */
    // https://stackoverflow.com/questions/27370480/how-to-initialize-an-stdstring-with-a-length
    std::string longCData(300, '?');
    // https://www.w3schools.com/cpp/cpp_strings_concat.asp
    std::string longstr("<tag>" + longCData + "</tag>");
    std::shared_ptr<CStringDataSource> longsrc = std::make_shared<CStringDataSource>(longstr);
    CXMLReader reader(longsrc);
    SXMLEntity entity;

    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "tag");

    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    // https://stackoverflow.com/questions/905479/why-does-stdstring-have-a-length-member-function-in-addition-to-size
    EXPECT_EQ(entity.DNameData.length(), 300);
    EXPECT_EQ(entity.DNameData, longCData);

    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "tag");
    
    EXPECT_TRUE(reader.End());

}

TEST(XMLReaderTest, SpecialCharacterTest){
    /* Tests for:
    * - Correct parsing of XML-encoded special characters (&amp;, &apos;, etc.)
    * - Accurate preservation of special characters in tag attributes
    * - Accurate preservation of special characters in CharData (text)
    * - Valid detection of start/end dlement entities with special character content
    */
    // https://www.baeldung.com/xml-encode-special-characters#bd-standard-xml-entities
    std::shared_ptr<CStringDataSource> src = std::make_shared<CStringDataSource>("<tag attr1=\"&amp;&apos;&gt;&lt;&quot;\">&amp;&apos;&gt;&lt;&quot;</tag>");
    CXMLReader reader(src);
    SXMLEntity entity;

    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "tag");

    EXPECT_EQ(entity.DAttributes.size(), 1);
    EXPECT_TRUE(entity.AttributeExists("attr1"));
    EXPECT_EQ(entity.AttributeValue("attr1"), "&'><\"");

    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "&'><\"");

    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "tag");

    EXPECT_TRUE(reader.End());

}

TEST(XMLReaderTest, InvalidXMLTest){
    /* Tests for:
    * - Handles incomplete XML elements
    * - Fails with mismatched end tags
    * - Fails with invalid start tag characters
    * - Handles incorrectly nested XML elements
    * - No crashes/invalid entities for improperly formed XML input
    */
    SXMLEntity entity;

    // Tests for incomplete elements, e.g. start element without end element
    std::shared_ptr<CStringDataSource> incompsrc = std::make_shared<CStringDataSource>("<tag>"); // only start elemetn without end
    CXMLReader incompreader(incompsrc);
    
    EXPECT_TRUE(incompreader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "tag");
    EXPECT_FALSE(incompreader.ReadEntity(entity, false)); // shouldn't read further because no end element

    // Tests for wrong tags
    std::shared_ptr<CStringDataSource> wrongsrc = std::make_shared<CStringDataSource>("<tag></wrong_tag>"); // wrong end element
    CXMLReader wrongreader(wrongsrc);

    EXPECT_TRUE(wrongreader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "tag");
    EXPECT_FALSE(wrongreader.ReadEntity(entity, false));

    // Tests for mismatched tagging
    std::shared_ptr<CStringDataSource> mismatchsrc = std::make_shared<CStringDataSource>("> <tag>");
    CXMLReader mismatchreader(mismatchsrc);

    EXPECT_FALSE(mismatchreader.ReadEntity(entity, false)); // shouldn't be able to read the first tag

    // Tests for incorrect nesting
    std::shared_ptr<CStringDataSource> nestingsrc = std::make_shared<CStringDataSource>("<tag1><tag2></tag1>");
    CXMLReader nestingreader(nestingsrc);

    EXPECT_TRUE(nestingreader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "tag1");
    EXPECT_TRUE(nestingreader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "tag2");
    EXPECT_FALSE(nestingreader.ReadEntity(entity, false)); // should fail because bar2 has not been closed

}

TEST(XMLReaderTest, LongCharDataCrosses512Boundary){
    /* Tests for:
    * - Correct parsing of extra-long CharData (600 characters) (exceeds buffer size)
    * - No truncation of text with multiple buffer reads
    * - Length and content validation of CharData
    * - End of XML detection for XML
    */
    std::string longCData(600, '?');
    std::string longstr = "<tag>" + longCData + "</tag>";
    std::shared_ptr<CStringDataSource> longsrc = std::make_shared<CStringDataSource>(longstr);
    CXMLReader reader(longsrc);
    SXMLEntity entity;

    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "tag");

    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData.length(), 600);
    EXPECT_EQ(entity.DNameData, longCData);

    EXPECT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "tag");

    EXPECT_TRUE(reader.End());

}

