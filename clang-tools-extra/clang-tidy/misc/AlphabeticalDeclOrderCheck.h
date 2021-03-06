//===--- AlphabeticalDeclOrderCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_ALPHABETICALDECLORDERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_ALPHABETICALDECLORDERCHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace misc {

/// AlphabeticalDeclOrderCheck 
///   Checks for alphabetical order on same line declarations. This provides
///   FIXIT hints as well.
/// Custom Added Check
class AlphabeticalDeclOrderCheck : public ClangTidyCheck {
public:
  AlphabeticalDeclOrderCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool checkAlphabeticalOrder(const DeclStmt *MatchedStmt, std::vector<StringRef> &DeclNames, bool &fixDecl);
};

} // namespace misc
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MISC_ALPHABETICALDECLORDERCHECK_H
