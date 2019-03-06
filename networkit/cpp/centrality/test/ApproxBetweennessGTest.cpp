/*
 * ApproxBetweennessGTest.cpp
 *
 *  Created on: 30.06.2014
 *      Author: moritzl
 */

#include <gtest/gtest.h>

#include "../../../include/networkit/centrality/ApproxBetweenness.hpp"
#include "../../../include/networkit/centrality/Betweenness.hpp"
#include "../../../include/networkit/generators/ErdosRenyiGenerator.hpp"
#include "../../../include/networkit/generators/DorogovtsevMendesGenerator.hpp"
#include "../../../include/networkit/distance/Diameter.hpp"

namespace NetworKit {

class ApproxBetweennessGTest : public testing::Test {};

TEST_F(ApproxBetweennessGTest, benchApproxDiameterErdos) {
	ErdosRenyiGenerator gen(1000,0.002);
	Graph G1 = gen.generate();
	ApproxBetweenness approx(G1, 0.05, 0.1);
	approx.run();
}

}
