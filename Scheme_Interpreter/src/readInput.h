/*
 * readInput.h
 *
 *  Created on: May 15, 2014
 *      Author: ajay
 */

#ifndef READINPUT_H_
#define READINPUT_H_
#include "ini_configuration.h"
#include "DFA.h"
#include "eNFA.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <set>
using namespace std;

void input(int argc, char const* argv[]);
void testMinimize(const ini::Configuration &configuration, string testNr);
void testProduct(const ini::Configuration &configuration, string testNr);
void testRegex_Enfa(const ini::Configuration &configuration, string testNr);
void testMod_Subset(const ini::Configuration &configuration, string testNr);
DFA getDFA(const ini::Configuration &configuration, string dfaNr);
eNFA getEnfa(const ini::Configuration &configuration, string eNFANr);


#endif /* READINPUT_H_ */
