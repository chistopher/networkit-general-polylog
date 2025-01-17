/*
 * GlobalCurveballBenchmark.cpp
 *
 *  Created on: 24.05.2018
 *      Author: Manuel Penschuck (networkit@manuel.jetzt)
 */

#include <gtest/gtest.h>

#include "GlobalCurveballBenchmark.h"

#include "../GlobalCurveball.h"
#include "../GlobalTradeSequence.h"

#include "../../generators/HyperbolicGenerator.h"
#include "../../generators/ErdosRenyiGenerator.h"


namespace NetworKit {

void GlobalCurveballBenchmark::checkWithGraph(NetworKit::Graph& G) {
    node numNodes = G.numberOfNodes();
    const count numTrades = 2;
    std::vector<node> degrees(numNodes + 1);

    // Add edge to node 0, if isolated node
    // If 0 itself is isolated, add new node and connect 0 to it
    G.forNodes([&](node u) {
        if (G.degree(u) > 0)
            degrees[u] = G.degree(u);
        else {
            if (u == 0) {
                numNodes++;
                G.addNode();
                G.addEdge(0, numNodes - 1);
                degrees[0]++;
                degrees[numNodes - 1] = 1;
            } else {
                G.addEdge(u, 0);
                degrees[0]++;
                degrees[u] = 1;
            }
        }
    });


    NetworKit::GlobalCurveball algo(G, numTrades);
    algo.run();

    // check degrees
    NetworKit::Graph outG = algo.getGraph();
    outG.forNodes([&](node u){
        ASSERT_EQ(degrees[u], outG.degree(u));
    });
}

TEST_F(GlobalCurveballBenchmark, benchmarkCurveballHyperbolic) {
    Aux::Random::setSeed(1, false);

    node numNodes = 1000000;
    NetworKit::HyperbolicGenerator generator(numNodes, 32);
    NetworKit::Graph G = generator.generate();

    this->checkWithGraph(G);
}

TEST_F(GlobalCurveballBenchmark, benchmarkErdosRenyiHyperbolic) {
    Aux::Random::setSeed(1, false);

    node numNodes = 1000000;
    NetworKit::ErdosRenyiGenerator generator(numNodes, 32. / numNodes);
    NetworKit::Graph G = generator.generate();

    this->checkWithGraph(G);
}


template<typename T>
static void benchmarkHash(const count n, const count r) {
    Aux::Random::setSeed(1, false);
    CurveballDetails::GlobalTradeSequence<T> hash{n, r, Aux::Random::getURNG()};

    Aux::Timer timer;
    timer.start();
    node tmp = 0;
    for (count ir=0; ir < r; ir++) {
        hash.switchToRound(r);
        for (count i = 0; i < n; i++) {
            tmp += hash.hash(i);
            tmp += hash.hashNext(i);
        }
    }
    timer.stop();

    const auto time = static_cast<double>(timer.elapsedNanoseconds());
    std::cout << "Time: " << time << "ns (" << (time / (2*r*n)) << "ns per Hash)\n";
    if (time == 1) {std::cout << tmp;}
}

TEST_F(GlobalCurveballBenchmark, benchmarkGlobalTradeSequence) {
    const count n = 100000;
    const count r = 10;

    benchmarkHash<CurveballDetails::LinearCongruentialMap<node> >(n, r);
}


}