//
// Created by Ethan Edwards on 11/1/2023.
//

#include "Graph.h"

#include <utility>
#include <chrono>

Graph::Graph() = default;

void Graph::buildGraph(const std::map<std::string, std::vector<std::string>> &requisiteMap) {
    for (auto&[key, value] : requisiteMap){
        CourseNode *courseNode = new CourseNode(key);
        if (graphMap.find(key) == graphMap.end()){
            graphMap[key] = courseNode;
        }
        for (auto & i : value){
            CourseNode *prereqNode = new CourseNode(i);
            if (graphMap.find(i) == graphMap.end()){
                graphMap[i] = prereqNode;
            }
            graphMap[key]->addPrereqNode(graphMap[i]);
        }
    }
}

void Graph::printGraph() {
    for (auto&[key, value] : graphMap){
        std::cout << key << ": ";
        int iter = 0;
        for (auto & i : value->prereqNodes()){
            std::cout << i->courseID();
            if (iter != value->prereqNodes().size() - 1){std::cout << ", ";} // If not last element, print comma
            iter++;
        }
        std::cout << std::endl;
    }
}

bool Graph::pathFromToHelper(std::string fromNodeLabel, std::string toNodeLabel, std::vector<std::string> &path) {
    if (fromNodeLabel == toNodeLabel){
        path.push_back(graphMap[fromNodeLabel]->courseID());
        graphMap[fromNodeLabel]->onPath() = true;
        return true;
    }
    if (graphMap[fromNodeLabel]->prereqNodes().empty() || graphMap[fromNodeLabel]->onPath()){
        return false;
    }
    for (auto & i : graphMap[fromNodeLabel]->prereqNodes()){
        if (pathFromToHelper(i->courseID(), toNodeLabel, path)){
            path.push_back(graphMap[fromNodeLabel]->courseID());
            graphMap[fromNodeLabel]->onPath() = true;
            return true;
        }
    }
    return false;
}

std::vector<std::string> Graph::pathFromTo(std::string fromNodeLabel, std::string toNodeLabel) {
    std::vector<std::string> path;
    pathFromToHelper(std::move(fromNodeLabel), std::move(toNodeLabel), path);
    return path;
}

bool Graph::canBeTakenConcurrently(std::string node1Label, std::string node2Label) { // TODO
    // NIGHTMARE NIGHTMARE NIGHTMARE
    return false;
}

void Graph::prereqGetter(std::string init, std::string courseID, std::vector<std::pair<bool, std::vector<std::string>>>& chains){
    if (graphMap[courseID]->prereqNodes().empty()){
        chains.emplace_back(true, std::vector<std::string>{courseID});
        return;
    }
    // Get the prerequisite chains of the current course
    std::vector<std::pair<bool, std::vector<std::string>>> myChains;
    for (auto & i : graphMap[courseID]->prereqNodes()){
        prereqGetter(init, i->courseID(), myChains);
    }
    //Add the current course to the end of each chain
    for (auto & i : myChains){
        i.second.push_back(courseID);
    }
    // Add the chains to the chains vector
    chains.insert(chains.end(), myChains.begin(), myChains.end());
}

void Graph::prerequisiteChainsFor(std::string courseID, std::vector<std::vector<std::string>> &prerequisiteChains) {
    std::vector<std::pair<bool, std::vector<std::string>>> chains;
    prereqGetter(courseID, courseID, chains);
    for (auto & i : chains){
        prerequisiteChains.push_back(i.second);
    }
}

bool Graph::isCyclic() { // TODO

}

int Graph::degreeOfDependency(std::string courseID) {
    int counter = 0;
    for (auto&[key, value] : graphMap){
        for (auto & i : value->prereqNodes()){
            if (i->courseID() == courseID){
                counter++;
            }
        }
    }
    return counter;
}

int Graph::longestChain() { // TODO
    int longest = 0;
    for (auto&[key, value] : graphMap){
        std::vector<std::vector<std::string>> prereqs;
        prerequisiteChainsFor(key, prereqs);
        for (auto & i : prereqs){
            if (i.size() > longest){
                longest = i.size();
            }
        }
    }
    return longest;
}

int Graph::longestChain(std::string courseID) {
    std::vector<std::vector<std::string>> prereqs;
    prerequisiteChainsFor(courseID, prereqs);
    int longest = 0;
    for (auto & i : prereqs){
        if (i.size() > longest){
            longest = static_cast<int>(i.size());
        }
    }
    return longest;
}

bool Graph::isALabel(std::string courseID) {
    return graphMap.find(courseID) != graphMap.end();
}

