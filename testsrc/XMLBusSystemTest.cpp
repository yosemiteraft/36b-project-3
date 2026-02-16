#include <gtest/gtest.h>
#include "XMLBusSystem.h"
#include "StringDataSource.h"

TEST(XMLBusSystemTest, SimpleTest){
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

    auto Path = BusSystem.PathByStopIDs(1,2);
    ASSERT_NE(Path, nullptr);
    EXPECT_EQ(Path->StartNodeID(), 321);
    EXPECT_EQ(Path->EndNodeID(), 311);
    EXPECT_EQ(Path->NodeCount(), 3);
    EXPECT_EQ(Path->GetNodeID(0), 321);
    EXPECT_EQ(Path->GetNodeID(1), 315);
    EXPECT_EQ(Path->GetNodeID(2), 311);
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
    EXPECT_NE(StopIndex0, nullptr);
    EXPECT_EQ(StopIndex0->ID(), 1);
    EXPECT_EQ(StopIndex0->NodeID(), 321);
    EXPECT_EQ(StopIndex0->Description(), "First");

    auto StopIndex1 = BusSystem.StopByIndex(1);
    EXPECT_NE(StopIndex1, nullptr);
    EXPECT_EQ(StopIndex1->ID(), 2);
    EXPECT_EQ(StopIndex1->NodeID(), 311);
    EXPECT_EQ(StopIndex1->Description(), "second");  
    
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
    EXPECT_EQ(StopObj->Description(), "First");
    EXPECT_EQ(StopObj->Description("Not First"), "Not First");
}

TEST(XMLBusSystemTest, ZeroStopsTest) {
    auto BusRouteSource = std::make_shared<CStringDataSource>(  "<bussystem>\n"
                                                                "<stops>\n"
                                                                "</stops>\n"
                                                                "</bussystem>\n");
    auto BusRouteReader = std::make_shared< CXMLReader >(BusRouteSource);
    CXMLBusSystem BusSystem(BusRouteReader,nullptr);
    
    EXPECT_EQ(BusSystem.StopCount(), 0);
    EXPECT_EQ(BusSystem.StopByIndex(0), nullptr);
}

TEST(XMLBusSystemTest, ZeroNodesTest) {
    auto BusPathSource = std::make_shared<CStringDataSource>(  "<paths>\n"
                                                                "   <path source=\"321\" destination=\"311\">\n"
                                                                "      <node id=\"321\"/>\n"
                                                                "      <node id=\"315\"/>\n"
                                                                "      <node id=\"311\"/>\n"
                                                                "   </path>\n"
                                                                "</paths>");
    auto BusPathReader = std::make_shared< CXMLReader >(BusPathSource);
    CXMLBusSystem BusSystem(nullptr,BusPathReader);

    auto Path = BusSystem.PathByStopIDs(1, 2);
    EXPECT_EQ(Path, nullptr);
}
