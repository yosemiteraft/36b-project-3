#include <gtest/gtest.h>
#include "OpenStreetMap.h"
#include "StringDataSource.h"

TEST(OpenStreetMapTest, SimpleTest){
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
}

TEST(OpenStreetMapTest, InvalidNodeByIndexTest){
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
    EXPECT_EQ(NodeID1->GetAttribute("test"), ""); 
    
    auto NodeID2 = OpenStreetMap.NodeByID(2);
    ASSERT_NE(NodeID2, nullptr);
    EXPECT_EQ(NodeID2->AttributeCount(), 0);
    EXPECT_FALSE(NodeID2->HasAttribute("test"));
    EXPECT_EQ(NodeID2->GetAttribute("test"), "");
}


