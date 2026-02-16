#include <gtest/gtest.h>
#include "XMLBusSystem.h"
#include "StringDataSource.h"

TEST(XMLBusSystemTest, SimpleTest){
    /* Tests for:
        - Correct parsing of simple bus route and bus path
        - StopByIndex with valid index returning a stop
        - StopByID with valid ID returning a stop
        - ID, NodeID, and Description of a stop
        - Search for path from start and end stop IDs
        - Start node, end node, and node count
        - Correctly getting node IDs
    */
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "      <node id=\"321\"/>\n"
                                                                "      <node id=\"315\"/>\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "   </path>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    ASSERT_EQ(BusSystem.StopCount(),2);
    EXPECT_EQ(BusSystem.RouteCount(),0);
    EXPECT_NE(BusSystem.StopByIndex(0),nullptr);
    EXPECT_NE(BusSystem.StopByIndex(1),nullptr);
    auto StopObj = BusSystem.StopByID(1);
    ASSERT_NE(StopObj,nullptr);
    EXPECT_EQ(StopObj->ID(),1);
    EXPECT_EQ(StopObj->NodeID(),321);
    EXPECT_EQ(StopObj->Description(),"First");
    StopObj = BusSystem.StopByID(2);
    ASSERT_NE(StopObj,nullptr);
    EXPECT_EQ(StopObj->ID(),2);
    EXPECT_EQ(StopObj->NodeID(),311);
    EXPECT_EQ(StopObj->Description(),"second");

    auto PathObj = BusSystem.PathByStopIDs(1,2);
    ASSERT_NE(PathObj, nullptr);
    EXPECT_EQ(PathObj->StartNodeID(), 321);
    EXPECT_EQ(PathObj->EndNodeID(), 311);
    EXPECT_EQ(PathObj->NodeCount(), 3);
    EXPECT_EQ(PathObj->GetNodeID(0), 321);
    EXPECT_EQ(PathObj->GetNodeID(1), 315);
    EXPECT_EQ(PathObj->GetNodeID(2), 311);
}

TEST(XMLBusSystemTest, StopByIndexTest) {
    
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    CXMLBusSystem BusSystem(BusRouteReader,nullptr);

    auto StopIndex0 = BusSystem.StopByIndex(0);
    ASSERT_NE(StopIndex0, nullptr);
    EXPECT_EQ(StopIndex0->ID(), 1);
    EXPECT_EQ(StopIndex0->NodeID(), 321);
    EXPECT_EQ(StopIndex0->Description(), "First");

    auto StopIndex1 = BusSystem.StopByIndex(1);
    ASSERT_NE(StopIndex1, nullptr);
    EXPECT_EQ(StopIndex1->ID(), 2);
    EXPECT_EQ(StopIndex1->NodeID(), 311);
    EXPECT_EQ(StopIndex1->Description(), "second");  
    
    // Invalid index
    auto StopBigIndex = BusSystem.StopByIndex(5);
    EXPECT_EQ(StopBigIndex, nullptr);

    auto StopNegIndex = BusSystem.StopByIndex(-5);
    EXPECT_EQ(StopNegIndex, nullptr);
}

TEST(XMLBusSystemTest, StopByIDTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    CXMLBusSystem BusSystem(BusRouteReader,nullptr);

    auto StopID1 = BusSystem.StopByID(1);
    EXPECT_EQ(StopID1->ID(), 1);
    EXPECT_EQ(StopID1->NodeID(), 321);
    EXPECT_EQ(StopID1->Description(), "First");

    // ID that isn't found
    auto stop_invalid_id = BusSystem.StopByID(123);
    EXPECT_EQ(stop_invalid_id, nullptr);
}

TEST(XMLBusSystemTest, SetDescriptionTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    CXMLBusSystem BusSystem(BusRouteReader,nullptr);

    auto StopObj = BusSystem.StopByID(1);
    ASSERT_NE(StopObj, nullptr);
    EXPECT_EQ(StopObj->Description(), "First");
    EXPECT_EQ(StopObj->Description("Not First"), "Not First");
}

TEST(XMLBusSystemTest, ZeroStopsTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    CXMLBusSystem BusSystem(BusRouteReader,nullptr);
    
    EXPECT_EQ(BusSystem.StopCount(), 0);
    EXPECT_EQ(BusSystem.StopByIndex(0), nullptr);
}

TEST(XMLBusSystemTest, ZeroNodesTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "   </path>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    auto PathObj = BusSystem.PathByStopIDs(1, 2);
    ASSERT_NE(PathObj, nullptr);
    EXPECT_EQ(PathObj->StartNodeID(), 321);
    EXPECT_EQ(PathObj->EndNodeID(), 311);
    EXPECT_EQ(PathObj->NodeCount(), 0);
}

TEST(XMLBusSystemTest, ZeroStopsZeroNodesTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "   </path>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    EXPECT_EQ(BusSystem.StopCount(), 0);
    auto StopObj = BusSystem.StopByIndex(0);
    EXPECT_EQ(StopObj, nullptr);

    auto PathObj = BusSystem.PathByStopIDs(0, 0);
    EXPECT_EQ(PathObj, nullptr);
}

TEST(XMLBusSystemTest, PathByStopIDsTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "   <stop id=\"3\" node=\"301\" description=\"third\"/>\n"
                                                                "   <stop id=\"4\" node=\"291\" description=\"fourth\"/>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "      <node id=\"321\"/>\n"
                                                                "      <node id=\"315\"/>\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "   </path>\n"
                                                                "   <path source=\"311\" destination=\"301\">\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "      <node id=\"317\"/>\n"
                                                                "      <node id=\"320\"/>\n"
                                                                "      <node id=\"301\"/>\n"
                                                                "   </path>\n"
                                                                "   <path source=\"301\" destination=\"291\">\n"
                                                                "   </path>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    auto PathObj1 = BusSystem.PathByStopIDs(1,2);
    ASSERT_NE(PathObj1, nullptr);
    EXPECT_EQ(PathObj1->NodeCount(), 3);
    EXPECT_EQ(PathObj1->StartNodeID(), 321);
    EXPECT_EQ(PathObj1->EndNodeID(), 311);

    auto PathObj2 = BusSystem.PathByStopIDs(2,3);
    ASSERT_NE(PathObj2, nullptr);
    EXPECT_EQ(PathObj2->NodeCount(), 4);
    EXPECT_EQ(PathObj2->StartNodeID(), 311);
    EXPECT_EQ(PathObj2->EndNodeID(), 301);

    auto PathObj_ZeroNodes = BusSystem.PathByStopIDs(3,4);
    ASSERT_NE(PathObj_ZeroNodes, nullptr);
    EXPECT_EQ(PathObj_ZeroNodes->NodeCount(), 0); // zero nodes within this path
    EXPECT_EQ(PathObj_ZeroNodes->StartNodeID(), 301);
    EXPECT_EQ(PathObj_ZeroNodes->EndNodeID(), 291);
}

TEST(XMLBusSystemTest, NoPathFoundTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "   <stop id=\"3\" node=\"301\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    auto PathNotFoundObj = BusSystem.PathByStopIDs(1,2);
    EXPECT_EQ(PathNotFoundObj, nullptr);
}

TEST(XMLBusSystemTest, InvalidPathStopIDTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "   <stop id=\"3\" node=\"301\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "      <node id=\"321\"/>\n"
                                                                "      <node id=\"315\"/>\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "   </path>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    auto PathInvalidStopIDObj = BusSystem.PathByStopIDs(1, 50);
    EXPECT_EQ(PathInvalidStopIDObj, nullptr);

    auto PathInvalidStopIDObj2 = BusSystem.PathByStopIDs(50, 1);
    EXPECT_EQ(PathInvalidStopIDObj2, nullptr);
}

TEST(XMLBusSystemTest, PathInvalidNodeTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "   <stop id=\"3\" node=\"301\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "      <node id=\"321\"/>\n"
                                                                "      <node id=\"315\"/>\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "   </path>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    auto PathObj = BusSystem.PathByStopIDs(1,2);
    ASSERT_NE(PathObj, nullptr);
    EXPECT_EQ(PathObj->GetNodeID(2), 311);
    EXPECT_EQ(PathObj->GetNodeID(5), 0); // index out of range
}

TEST(XMLBusSystemTest, RouteTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "   <stop id=\"3\" node=\"301\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "<routes>\n"
                                                                "   <route name=\"A\">\n"
                                                                        "<routestop stop=\"1\" />\n"
                                                                        "<routestop stop=\"2\" />\n"
                                                                "   </route>\n"
                                                                "   <route name=\"B\">\n"
                                                                        "<routestop stop=\"2\" />\n"
                                                                        "<routestop stop=\"3\" />\n"
                                                                "   </route>\n"
                                                                "</routes>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(   "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "      <node id=\"321\"/>\n"
                                                                "      <node id=\"315\"/>\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "   </path>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    EXPECT_EQ(BusSystem.RouteCount(), 2);

    auto RouteObjByIndex = BusSystem.RouteByIndex(0);
    ASSERT_NE(RouteObjByIndex, nullptr);
    EXPECT_EQ(RouteObjByIndex->Name(), "A");
    EXPECT_EQ(RouteObjByIndex->StopCount(), 2);
    EXPECT_EQ(RouteObjByIndex->GetStopID(0), 1);
    EXPECT_EQ(RouteObjByIndex->GetStopID(1), 2);

    auto RouteObjByName = BusSystem.RouteByName("B");
    ASSERT_NE(RouteObjByName, nullptr);
    EXPECT_EQ(RouteObjByName->Name(), "B");
    EXPECT_EQ(RouteObjByName->StopCount(), 2);
    EXPECT_EQ(RouteObjByName->GetStopID(0), 2);
    EXPECT_EQ(RouteObjByName->GetStopID(1), 3);
}

TEST(XMLBusSystemTest, RouteByInvalidIndexTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "   <stop id=\"3\" node=\"301\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "<routes>\n"
                                                                "   <route name=\"A\">\n"
                                                                        "<routestop stop=\"1\" />\n"
                                                                        "<routestop stop=\"2\" />\n"
                                                                "   </route>\n"
                                                                "</routes>\n"
                                                                "</bussystem>\n");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(   "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "      <node id=\"321\"/>\n"
                                                                "      <node id=\"315\"/>\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    auto RouteObjByIndex = BusSystem.RouteByIndex(2);
    EXPECT_EQ(RouteObjByIndex, nullptr);

    auto RouteObjByNegIndex = BusSystem.RouteByIndex(-5);
    EXPECT_EQ(RouteObjByNegIndex, nullptr);
}

TEST(XMLBusSystemTest, RouteByInvalidNameTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "   <stop id=\"3\" node=\"301\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "<routes>\n"
                                                                "   <route name=\"A\">\n"
                                                                        "<routestop stop=\"1\" />\n"
                                                                        "<routestop stop=\"2\" />\n"
                                                                "   </route>\n"
                                                                "</routes>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(   "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "      <node id=\"321\"/>\n"
                                                                "      <node id=\"315\"/>\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    auto RouteObjByInvalidName = BusSystem.RouteByName("B");
    EXPECT_EQ(RouteObjByInvalidName, nullptr);
}

TEST(XMLBusSystemTest, RouteGetInvalidStopIDTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "   <stop id=\"3\" node=\"301\" description=\"second\"/>\n"
                                                                "</stops>\n"
                                                                "<routes>\n"
                                                                "   <route name=\"A\">\n"
                                                                        "<routestop stop=\"1\" />\n"
                                                                        "<routestop stop=\"2\" />\n"
                                                                "   </route>\n"
                                                                "</routes>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(   "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "      <node id=\"321\"/>\n"
                                                                "      <node id=\"315\"/>\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    auto RouteObjByIndex = BusSystem.RouteByIndex(0);
    ASSERT_NE(RouteObjByIndex, nullptr);
    EXPECT_EQ(RouteObjByIndex->GetStopID(2), CBusSystem::InvalidStopID);
    EXPECT_EQ(RouteObjByIndex->GetStopID(-5), CBusSystem::InvalidStopID);
}

TEST(XMLBusSystemTest, WrongTagsTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "   <stop id=\"1\" node=\"321\" description=\"First\"/>\n"
                                                                "   <stop id=\"2\" node=\"311\" description=\"second\"/>\n"
                                                                "   <stop id=\"3\" node=\"301\" description=\"second\"/>\n"
                                                                "</stop>\n" // wrong closing tag
                                                                "<routes>\n"
                                                                "   <route name=\"A\">\n"
                                                                        "<routestop stop=\"1\" />\n"
                                                                        "<routestop stop=\"2\" />\n"
                                                                // no closing tag
                                                                "</routes>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(   // no start tag
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "      <node id=\"321\"/>\n"
                                                                "      <node id=\"315\"/>\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    // Return empty or null results when faulty tags are given
    EXPECT_EQ(BusSystem.StopCount(), 0);
    EXPECT_EQ(BusSystem.RouteCount(), 0);
    EXPECT_EQ(BusSystem.StopByIndex(0), nullptr);
}

TEST(XMLBusSystemTest, EmptySourceTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                    "<stops>\n"
                                                                    "</stops>\n"
                                                                    "<routes>\n"
                                                                    "</routes>\n"
                                                                "</bussystem>");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    auto BusPathSource = std::make_shared<CStringDataSource>(   "<paths>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(BusRouteReader,BusPathReader);

    EXPECT_EQ(BusSystem.StopCount(), 0);
    EXPECT_EQ(BusSystem.RouteCount(), 0);
    EXPECT_EQ(BusSystem.StopByIndex(0), nullptr);
    EXPECT_EQ(BusSystem.PathByStopIDs(0,1), nullptr);

    auto RouteObjEmpty = BusSystem.RouteByIndex(0);
    EXPECT_EQ(RouteObjEmpty, nullptr);
}









