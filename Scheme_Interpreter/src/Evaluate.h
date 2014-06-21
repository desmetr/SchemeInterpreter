#ifndef EVALUATE_H_
#define EVALUATE_H_

#include <memory>
#include <iostream>
#include <string>
#include <list>

#include "Expression.h"
#include "Environment.h"
#include "Lambda.h"

Expression evaluate(const Expression& exp, std::shared_ptr<Environment> envptr);


#endif
