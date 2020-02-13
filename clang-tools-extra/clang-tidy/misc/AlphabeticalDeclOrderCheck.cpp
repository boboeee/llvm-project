//===--- AlphabeticalDeclOrderCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AlphabeticalDeclOrderCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace misc {

void AlphabeticalDeclOrderCheck::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  Finder->addMatcher(declStmt().bind("stmt"), this);
}

bool AlphabeticalDeclOrderCheck::checkAlphabeticalOrder(const DeclStmt *MatchedStmt, std::vector<StringRef> &DeclNames, bool &fixDecl) {
  auto prev_it = MatchedStmt->decl_begin();
  auto it = MatchedStmt->decl_begin();
  auto firstVarDecl = dyn_cast<VarDecl>(*MatchedStmt->decl_begin());
  bool found = true;
  

  
  DeclNames.push_back(firstVarDecl->getName());
  ++it;
  
  while (it != MatchedStmt->decl_end()) {
    auto varDecl = dyn_cast<VarDecl>(*it);
    auto prevVarDecl = dyn_cast<VarDecl>(*prev_it);

    if (varDecl->getName() < prevVarDecl->getName())
      found = false;
    if (varDecl->getType() != prevVarDecl->getType())
      fixDecl = false;

    DeclNames.push_back(varDecl->getName());
    prev_it = it;
    ++it;
  }
  return found;
}

void AlphabeticalDeclOrderCheck::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
  std::vector<StringRef> ret;
  const auto *MatchedStmt = Result.Nodes.getNodeAs<DeclStmt>("stmt");
  bool fixDecl = true;

  if (MatchedStmt->isSingleDecl())
    return;

  auto firstVarDecl = dyn_cast<VarDecl>(*MatchedStmt->decl_begin());
  if (firstVarDecl ==  NULL) {
    return;  
  }

  if (!checkAlphabeticalOrder(MatchedStmt, ret, fixDecl)) {

    auto MyDiag = diag(MatchedStmt->getBeginLoc(), "declaration name is out of order");
    if (fixDecl == false)
      return;

    std:: sort(ret.begin(), ret.end());
    auto itStmt = MatchedStmt->decl_begin();
    auto itVec = ret.begin();
    for (;itStmt != MatchedStmt->decl_end() && itVec != ret.end();++itStmt, ++itVec) {
      auto varDecl = dyn_cast<VarDecl>(*itStmt);
      if (varDecl->getName() == *itVec)
        continue;

      SourceRange RemovalRange(varDecl->getLocation());

      MyDiag << FixItHint::CreateReplacement(RemovalRange, (*itVec).str());

    }
  }
  
}

} // namespace misc
} // namespace tidy
} // namespace clang
