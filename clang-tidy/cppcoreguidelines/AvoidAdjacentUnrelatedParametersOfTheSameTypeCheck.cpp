//===--- AvoidAdjacentUnrelatedParametersOfTheSameTypeCheck.cpp - clang-tidy-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "AvoidAdjacentUnrelatedParametersOfTheSameTypeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace cppcoreguidelines {

void AvoidAdjacentUnrelatedParametersOfTheSameTypeCheck::registerMatchers(MatchFinder *Finder) {
  auto hasAtLeastTwoParameters = unless(anyOf(parameterCountIs(0), parameterCountIs(1)));
  auto isLambdaInternalInvoke = allOf(hasName("__invoke"), hasParent(cxxRecordDecl(isLambda())));
  // auto isFreeComparisionFunction = allOf(returns(booleanType()),
  //                                    parameterCountIs(2),
  //                                    hasAnyName("operator==", "operator!=", "operator>",
  //                                               "operator<", "operator<=", "operator>=")
  //                                   );

  auto constReferenceType = referenceType(pointee(isConstQualified()));
  auto isComparator = allOf(returns(booleanType()),
                            parameterCountIs(2),
                            hasParameter(0, hasType(constReferenceType)),
                            hasParameter(1, hasType(constReferenceType))
                            );

  Finder->addMatcher(
          functionDecl(hasAtLeastTwoParameters,
                        unless(anyOf(isLambdaInternalInvoke,
                                     isComparator))
                      ).bind("x"),
          this);
}

void AvoidAdjacentUnrelatedParametersOfTheSameTypeCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("x");

  for(auto leftIt = MatchedDecl->parameters().begin(),
          rightIt = std::next(leftIt); rightIt != MatchedDecl->parameters().end(); leftIt = rightIt++)
  {
    auto leftType = (*leftIt)->getType();
    auto rightType = (*rightIt)->getType();
    if(leftType == rightType)
    {
        diag(MatchedDecl->getLocation(), "function %0 has two adjacent parameters of the same type") << MatchedDecl;
        return;
    }
  }
}

} // namespace cppcoreguidelines
} // namespace tidy
} // namespace clang
