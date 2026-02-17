#include <gtest/gtest.h>
#include "OpenStreetMap.h"
#include "StringDataSource.h"
using std::cout;
using std::endl;

TEST(OpenStreetMapTest, SimpleTest){
    /* Tests for:
        - Correct parsing of simple node and way
        - NodeByIndex with valid index returning a node
        - Returning correct ID and location of node
        - Returning correct number of ways
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\"/>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                            "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    ASSERT_EQ(OpenStreetMap.NodeCount(), 2);
    auto Node = OpenStreetMap.NodeByIndex(0);
    ASSERT_NE(Node,nullptr);
    EXPECT_EQ(Node->ID(),1);
    auto Location = CStreetMap::SLocation{38.5,-121.7};
    EXPECT_EQ(Node->Location(),Location);
    ASSERT_EQ(OpenStreetMap.WayCount(), 1);
}

TEST(OpenStreetMapTest, NodeByIndexTest){
    /* Tests for:
        - NodeByIndex with valid index returning correct node
        - Node's ID, Location, and AttributeCount properties
        - GetAttribute returns correct attribute string
        - GetAttributeKey returns correct key string when passed valid index
        - HasAttribute returns correct boolean value when passed valid and invalid keys
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
                                                                "    <tag k=\"highway\" v=\"motorway_junction\"/>\n"
                                                                "    <tag k=\"ref\" v=\"71\"/>\n"
                                                                "  </node>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\">\n"
                                                                "    <tag k=\"foot\" v=\"yes\"/>\n"
		                                                        "    <tag k=\"barrier\" v=\"gate\"/>\n"
		                                                        "    <tag k=\"bicycle\" v=\"yes\"/>\n"
                                                                "  </node>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                            "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.NodeCount(), 2);

    auto NodeIndex0 = OpenStreetMap.NodeByIndex(0);
    ASSERT_NE(NodeIndex0, nullptr);
    EXPECT_EQ(NodeIndex0->ID(), 1);
    EXPECT_EQ(NodeIndex0->Location().DLatitude, 38.5);
    EXPECT_EQ(NodeIndex0->Location().DLongitude, -121.7);
    EXPECT_EQ(NodeIndex0->AttributeCount(), 2);
    EXPECT_EQ(NodeIndex0->GetAttribute("highway"), "motorway_junction");
    EXPECT_EQ(NodeIndex0->GetAttribute("ref"), "71");
    EXPECT_EQ(NodeIndex0->GetAttributeKey(0), "highway");
    EXPECT_EQ(NodeIndex0->GetAttributeKey(1), "ref");
    EXPECT_TRUE(NodeIndex0->HasAttribute("highway"));     
    EXPECT_TRUE(NodeIndex0->HasAttribute("ref"));   
    EXPECT_FALSE(NodeIndex0->HasAttribute("test"));                                               
    
    auto NodeIndex1 = OpenStreetMap.NodeByIndex(1);
    ASSERT_NE(NodeIndex1, nullptr);
    EXPECT_EQ(NodeIndex1->ID(), 2);
    EXPECT_EQ(NodeIndex1->Location().DLatitude, 38.5);
    EXPECT_EQ(NodeIndex1->Location().DLongitude, -121.8);
    EXPECT_EQ(NodeIndex1->AttributeCount(), 3);
    EXPECT_EQ(NodeIndex1->GetAttribute("foot"), "yes");
    EXPECT_EQ(NodeIndex1->GetAttribute("barrier"), "gate");
    EXPECT_EQ(NodeIndex1->GetAttribute("bicycle"), "yes");
    EXPECT_EQ(NodeIndex1->GetAttributeKey(0), "foot");
    EXPECT_EQ(NodeIndex1->GetAttributeKey(1), "barrier");
    EXPECT_EQ(NodeIndex1->GetAttributeKey(2), "bicycle");
    EXPECT_TRUE(NodeIndex1->HasAttribute("foot")); 
    EXPECT_TRUE(NodeIndex1->HasAttribute("barrier")); 
    EXPECT_TRUE(NodeIndex1->HasAttribute("bicycle")); 
}

TEST(OpenStreetMapTest, NodeByIDTest){
    /* Tests for:
        - NodeByID with valid ID returning the correct node
        - Node's ID, Location, and AttributeCount properties
        - GetAttribute returns correct attribute value when passed valid key
        - GetAttributeKey returns correct key string when passed valid index
        - HasAttribute returns correct boolean value when passed valid and invalid keys
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
                                                                "    <tag k=\"highway\" v=\"motorway_junction\"/>\n"
                                                                "    <tag k=\"ref\" v=\"71\"/>\n"
                                                                "  </node>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\">\n"
                                                                "    <tag k=\"foot\" v=\"yes\"/>\n"
		                                                        "    <tag k=\"barrier\" v=\"gate\"/>\n"
		                                                        "    <tag k=\"bicycle\" v=\"yes\"/>\n"
                                                                "  </node>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                            "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    auto NodeID1 = OpenStreetMap.NodeByID(1);
    ASSERT_NE(NodeID1, nullptr);
    EXPECT_EQ(NodeID1->ID(), 1);
    EXPECT_EQ(NodeID1->Location().DLatitude, 38.5);
    EXPECT_EQ(NodeID1->Location().DLongitude, -121.7);
    EXPECT_EQ(NodeID1->AttributeCount(), 2);
    EXPECT_EQ(NodeID1->GetAttribute("highway"), "motorway_junction");
    EXPECT_EQ(NodeID1->GetAttribute("ref"), "71");  
    EXPECT_EQ(NodeID1->GetAttributeKey(0), "highway");      
    EXPECT_EQ(NodeID1->GetAttributeKey(1), "ref");   
    EXPECT_TRUE(NodeID1->HasAttribute("highway")); 
    EXPECT_TRUE(NodeID1->HasAttribute("ref"));     
    EXPECT_FALSE(NodeID1->HasAttribute("test"));                                              
}

TEST(OpenStreetMapTest, InvalidNodeByIndexTest){
    /* Tests for:
        - NodeByIndex returns nullptr when passed an invalid index
            - Invalid index including out of range and negative index
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
                                                                "    <tag k=\"highway\" v=\"motorway_junction\"/>\n"
                                                                "    <tag k=\"ref\" v=\"71\"/>\n"
                                                                "  </node>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\">\n"
                                                                "    <tag k=\"foot\" v=\"yes\"/>\n"
		                                                        "    <tag k=\"barrier\" v=\"gate\"/>\n"
		                                                        "    <tag k=\"bicycle\" v=\"yes\"/>\n"
                                                                "  </node>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                            "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);
    
    auto NodeInvalidIndex = OpenStreetMap.NodeByIndex(2);
    EXPECT_EQ(NodeInvalidIndex, nullptr);

    auto NodeNegIndex = OpenStreetMap.NodeByIndex(-5);
    EXPECT_EQ(NodeNegIndex, nullptr);
}

TEST(OpenStreetMapTest, InvalidNodeByIDTest) {
    /* Tests for:
        - NodeByID returns nullptr when passed invalid ID
            - Invalid ID including nonexistent IDs
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
                                                                "    <tag k=\"highway\" v=\"motorway_junction\"/>\n"
                                                                "    <tag k=\"ref\" v=\"71\"/>\n"
                                                                "  </node>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\">\n"
                                                                "    <tag k=\"foot\" v=\"yes\"/>\n"
		                                                        "    <tag k=\"barrier\" v=\"gate\"/>\n"
		                                                        "    <tag k=\"bicycle\" v=\"yes\"/>\n"
                                                                "  </node>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                            "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    auto NodeInvalidID = OpenStreetMap.NodeByID(3);
    EXPECT_EQ(NodeInvalidID, nullptr);

    auto NodeNegID = OpenStreetMap.NodeByID(-5);
    EXPECT_EQ(NodeNegID, nullptr);
}

TEST(OpenStreetMapTest, NodeAttrEdgeCasesTest) {
    /* Tests for:
        - Edge cases dealing with node attributes
        - GetAttributeKey returns empty string when passed index that goes out of range
        - GetAttribute returns empty string when passed invalid key
        - Node with no attributes should have AttributeCount of 0
        - HasAttribute returns false when passed a nonexistent key
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
                                                                "    <tag k=\"highway\" v=\"motorway_junction\"/>\n"
                                                                "    <tag k=\"ref\" v=\"71\"/>\n"
                                                                "  </node>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\"/>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                            "</osm>"
                                                        );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    auto NodeID1 = OpenStreetMap.NodeByID(1);
    ASSERT_NE(NodeID1, nullptr);
    EXPECT_EQ(NodeID1->GetAttributeKey(2), ""); 
    EXPECT_EQ(NodeID1->GetAttributeKey(2000), ""); 
    EXPECT_EQ(NodeID1->GetAttribute("test"), ""); 
    
    auto NodeID2 = OpenStreetMap.NodeByID(2);
    ASSERT_NE(NodeID2, nullptr);
    EXPECT_EQ(NodeID2->AttributeCount(), 0);
    EXPECT_FALSE(NodeID2->HasAttribute("test"));
    EXPECT_EQ(NodeID2->GetAttribute("test"), "");
}

TEST(OpenStreetMapTest, WayByIndexTest) {
    /* Tests for:
        - WayByIndex returning correct way when passed valid index
        - Way's ID, NodeCount, and AttributeCount properties
        - GetNodeID returns correct NodeID when passed valid index
        - GetAttribute with valid/invalid key
        - GetAttributeKey with valid/invalid index
        - HasAttribute with valid/invalid key
        - Way with no attributes has AttributeCount of 0
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
                                                                "    <tag k=\"highway\" v=\"motorway_junction\"/>\n"
                                                                "    <tag k=\"ref\" v=\"71\"/>\n"
                                                                "  </node>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\">\n"
                                                                "    <tag k=\"foot\" v=\"yes\"/>\n"
		                                                        "    <tag k=\"barrier\" v=\"gate\"/>\n"
		                                                        "    <tag k=\"bicycle\" v=\"yes\"/>\n"
                                                                "  </node>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "    <tag k=\"highway\" v=\"residential\"/>\n"
		                                                        "    <tag k=\"name\" v=\"Rutgers Drive\"/>\n"
                                                                "  </way>\n"
                                                                "  <way id=\"2000\">\n"
                                                                "    <nd ref=\"3\"/>\n"
		                                                        "    <nd ref=\"4\"/>\n"
                                                                "    <nd ref=\"5\"/>\n"
                                                                "  </way>\n"
                                                            "</osm>"
                                                        );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.WayCount(), 2);

    auto WayIndex0 = OpenStreetMap.WayByIndex(0);
    ASSERT_NE(WayIndex0, nullptr);
    EXPECT_EQ(WayIndex0->ID(), 1000);
    EXPECT_EQ(WayIndex0->NodeCount(), 2);
    EXPECT_EQ(WayIndex0->GetNodeID(0), 1);
    EXPECT_EQ(WayIndex0->AttributeCount(), 2);
    EXPECT_EQ(WayIndex0->GetAttribute("highway"), "residential");
    EXPECT_EQ(WayIndex0->GetAttribute("name"), "Rutgers Drive");
    EXPECT_EQ(WayIndex0->GetAttribute("test"), "");
    EXPECT_EQ(WayIndex0->GetAttributeKey(0), "highway");
    EXPECT_EQ(WayIndex0->GetAttributeKey(1), "name");
    EXPECT_EQ(WayIndex0->GetAttributeKey(2), "");
    EXPECT_TRUE(WayIndex0->HasAttribute("highway"));
    EXPECT_TRUE(WayIndex0->HasAttribute("name"));
    EXPECT_FALSE(WayIndex0->HasAttribute("test"));

    auto WayIndex1 = OpenStreetMap.WayByIndex(1);
    ASSERT_NE(WayIndex1, nullptr);
    EXPECT_EQ(WayIndex1->ID(), 2000);
    EXPECT_EQ(WayIndex1->NodeCount(), 3);
    EXPECT_EQ(WayIndex1->AttributeCount(), 0);
}

TEST(OpenStreetMapTest, WayByInvalidIndexTest) {
    /* Tests for:
        - WayByIndex returns nullptr when passed invalid index
            - Invalid index including out of range and negative index
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
                                                                "    <tag k=\"highway\" v=\"motorway_junction\"/>\n"
                                                                "    <tag k=\"ref\" v=\"71\"/>\n"
                                                                "  </node>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\">\n"
                                                                "    <tag k=\"foot\" v=\"yes\"/>\n"
		                                                        "    <tag k=\"barrier\" v=\"gate\"/>\n"
		                                                        "    <tag k=\"bicycle\" v=\"yes\"/>\n"
                                                                "  </node>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                            "</osm>"
                                                        );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    auto WayInvalidIndex = OpenStreetMap.WayByIndex(2);      
    EXPECT_EQ(WayInvalidIndex, nullptr);
    
    auto WayNegIndex = OpenStreetMap.WayByIndex(-5);
    EXPECT_EQ(WayNegIndex, nullptr);
}

TEST(OpenStreetMapTest, WayByIDTest) {
    /* Tests for:
        - WayByID returning correct way when passed valid ID
        - Way's ID and NodeCount properties
        - GetNodeID with valid index
        - GetAttribute with valid/invalid key
        - GetAttributeKey with valid/invalid index
        - HasAttribute with valid/invalid key
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
                                                                "    <tag k=\"highway\" v=\"motorway_junction\"/>\n"
                                                                "    <tag k=\"ref\" v=\"71\"/>\n"
                                                                "  </node>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\">\n"
                                                                "    <tag k=\"foot\" v=\"yes\"/>\n"
		                                                        "    <tag k=\"barrier\" v=\"gate\"/>\n"
		                                                        "    <tag k=\"bicycle\" v=\"yes\"/>\n"
                                                                "  </node>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "    <tag k=\"highway\" v=\"residential\"/>\n"
		                                                        "    <tag k=\"name\" v=\"Rutgers Drive\"/>\n"
                                                                "  </way>\n"
                                                                "  <way id=\"2000\">\n"
                                                                "    <nd ref=\"3\"/>\n"
		                                                        "    <nd ref=\"4\"/>\n"
                                                                "    <nd ref=\"5\"/>\n"
                                                                "  </way>\n"
                                                            "</osm>"
                                                            );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    auto WayID1000 = OpenStreetMap.WayByID(1000);
    EXPECT_NE(WayID1000, nullptr);
    EXPECT_EQ(WayID1000->ID(), 1000);
    EXPECT_EQ(WayID1000->NodeCount(), 2);
    EXPECT_EQ(WayID1000->GetNodeID(0), 1);
    EXPECT_EQ(WayID1000->GetAttribute("highway"), "residential");
    EXPECT_EQ(WayID1000->GetAttribute("name"), "Rutgers Drive");
    EXPECT_EQ(WayID1000->GetAttribute("test"), "");
    EXPECT_EQ(WayID1000->GetAttributeKey(0), "highway");
    EXPECT_EQ(WayID1000->GetAttributeKey(1), "name");
    EXPECT_EQ(WayID1000->GetAttributeKey(2), "");
    EXPECT_TRUE(WayID1000->HasAttribute("highway"));
    EXPECT_TRUE(WayID1000->HasAttribute("name"));
    EXPECT_FALSE(WayID1000->HasAttribute("test"));

    auto WayID2000 = OpenStreetMap.WayByID(2000);
    EXPECT_NE(WayID2000, nullptr);
    EXPECT_EQ(WayID2000->NodeCount(), 3);
    EXPECT_EQ(WayID2000->GetNodeID(1), 4);
}

TEST(OpenStreetMapTest, WayByInvalidIDTest) {
    /* Tests for:
        - WayByID returns nullptr when passed invalid ID
            - Invalid ID including nonexistent ID
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
                                                                "    <tag k=\"highway\" v=\"motorway_junction\"/>\n"
                                                                "    <tag k=\"ref\" v=\"71\"/>\n"
                                                                "  </node>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\">\n"
                                                                "    <tag k=\"foot\" v=\"yes\"/>\n"
		                                                        "    <tag k=\"barrier\" v=\"gate\"/>\n"
		                                                        "    <tag k=\"bicycle\" v=\"yes\"/>\n"
                                                                "  </node>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                            "</osm>"
                                                        );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    auto WayID3000 = OpenStreetMap.WayByID(3000);
    EXPECT_EQ(WayID3000, nullptr);

    auto WayNegID = OpenStreetMap.WayByID(-2000);
    EXPECT_EQ(WayNegID, nullptr);
}

TEST(OpenStreetMapTest, InvalidNodeIDTest) {
    /* Tests for:
        - GetNodeID returning InvalidNodeID for invalid index
            - Out of bounds index
            - Negative index
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
                                                                "    <tag k=\"highway\" v=\"motorway_junction\"/>\n"
                                                                "    <tag k=\"ref\" v=\"71\"/>\n"
                                                                "  </node>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\">\n"
                                                                "    <tag k=\"foot\" v=\"yes\"/>\n"
		                                                        "    <tag k=\"barrier\" v=\"gate\"/>\n"
		                                                        "    <tag k=\"bicycle\" v=\"yes\"/>\n"
                                                                "  </node>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                            "</osm>"
                                                        );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    auto Way = OpenStreetMap.WayByIndex(0);
    EXPECT_EQ(Way->GetNodeID(2), CStreetMap::InvalidNodeID);
    EXPECT_EQ(Way->GetNodeID(-5), CStreetMap::InvalidNodeID);                                                    
}

TEST(OpenStreetMapTest, EmptyOSM) {
    /* Tests for:
        - Empty OSM source containing zero nodes and zero ways
        - NodeByIndex returns nullptr
        - WayByIndex returns nullptr
        - NodeCount and WayCount are 0
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                        "</osm>"
                                                        );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    auto Node = OpenStreetMap.NodeByIndex(0);
    EXPECT_EQ(Node, nullptr);

    auto Way = OpenStreetMap.WayByIndex(0);
    EXPECT_EQ(Way, nullptr);

    EXPECT_EQ(OpenStreetMap.NodeCount(), 0);
    EXPECT_EQ(OpenStreetMap.WayCount(), 0);
}

TEST(OpenStreetMapTest, WayZeroNdsTest) {
    /* Tests for:
        - Way with no <nd> elements
        - Counts correct number of ways but NodeCount is 0
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
                                                                "    <tag k=\"highway\" v=\"motorway_junction\"/>\n"
                                                                "    <tag k=\"ref\" v=\"71\"/>\n"
                                                                "  </node>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\">\n"
                                                                "    <tag k=\"foot\" v=\"yes\"/>\n"
		                                                        "    <tag k=\"barrier\" v=\"gate\"/>\n"
		                                                        "    <tag k=\"bicycle\" v=\"yes\"/>\n"
                                                                "  </node>\n"
                                                                "  <way id=\"1000\">\n"
                                                                "  </way>\n"
                                                            "</osm>"
                                                        );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.WayCount(), 1);
    auto Way = OpenStreetMap.WayByIndex(0);
    ASSERT_NE(Way, nullptr);
    EXPECT_EQ(Way->NodeCount(), 0);
}

TEST(OpenStreetMapTest, WrongTagsTest) {
    /* Tests for:
        - XML that is improperly formatted
        - Nodes before error are parsed
        - Everything afterwards is not parsed
        - Anything that was parsed has valid properties
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
                                                                "    <tag k=\"highway\" v=\"motorway_junction\"/>\n"
                                                                "    <tag k=\"ref\" v=\"71\"/>\n"
                                                                // missing closing tag
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\">\n"
                                                                "    <tag k=\"foot\" v=\"yes\"/>\n"
		                                                        "    <tag k=\"barrier\" v=\"gate\"/>\n"
		                                                        "    <tag k=\"bicycle\" v=\"yes\"/>\n"
                                                                "  </nod>\n" // incorrect closing tag
                                                                "  <way id=\"1000\">\n"
                                                                "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                // missing closing tag
                                                            "</osm>"
                                                        );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    std::cout << "NodeCount: " << OpenStreetMap.NodeCount() << std::endl;
    std::cout << "RouteCount: " << OpenStreetMap.WayCount() << std::endl;
    EXPECT_EQ(OpenStreetMap.NodeCount(), 1); 
    EXPECT_EQ(OpenStreetMap.WayCount(), 0); // should fail before parsing way

    auto Node = OpenStreetMap.NodeByIndex(0);
    ASSERT_NE(Node, nullptr);
    EXPECT_EQ(Node->ID(), 1);
    EXPECT_EQ(Node->Location().DLatitude, 38.5);
    EXPECT_EQ(Node->Location().DLongitude, -121.7);
}

TEST(OpenStreetMapTest, WrongOSMTagTest) {
    /* Tests for:
        - OSM source that has an incorrect start tag
        - NodeCount is 0 when OSM start tag was not found
            - Node not able to be parsed
    */
    auto OSMSource = std::make_shared<CStringDataSource>(  "<wrong version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\">\n"
                                                            "</osm>"
                                                        );
    auto OSMReader = std::make_shared< CXMLReader >(OSMSource);
    COpenStreetMap OpenStreetMap(OSMReader);

    EXPECT_EQ(OpenStreetMap.NodeCount(), 0);
}
