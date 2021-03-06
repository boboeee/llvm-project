//===--- OpenMPKinds.cpp - Token Kinds Support ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
/// This file implements the OpenMP enum and support functions.
///
//===----------------------------------------------------------------------===//

#include "clang/Basic/OpenMPKinds.h"
#include "clang/Basic/IdentifierTable.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/ErrorHandling.h"
#include <cassert>

using namespace clang;
using namespace llvm::omp;

OpenMPContextSelectorSetKind
clang::getOpenMPContextSelectorSet(llvm::StringRef Str) {
  return llvm::StringSwitch<OpenMPContextSelectorSetKind>(Str)
#define OPENMP_CONTEXT_SELECTOR_SET(Name) .Case(#Name, OMP_CTX_SET_##Name)
#include "clang/Basic/OpenMPKinds.def"
      .Default(OMP_CTX_SET_unknown);
}

llvm::StringRef
clang::getOpenMPContextSelectorSetName(OpenMPContextSelectorSetKind Kind) {
  switch (Kind) {
  case OMP_CTX_SET_unknown:
    return "unknown";
#define OPENMP_CONTEXT_SELECTOR_SET(Name)                                      \
  case OMP_CTX_SET_##Name:                                                     \
    return #Name;
#include "clang/Basic/OpenMPKinds.def"
    break;
  }
  llvm_unreachable("Invalid OpenMP context selector set kind");
}

OpenMPContextSelectorKind clang::getOpenMPContextSelector(llvm::StringRef Str) {
  return llvm::StringSwitch<OpenMPContextSelectorKind>(Str)
#define OPENMP_CONTEXT_SELECTOR(Name) .Case(#Name, OMP_CTX_##Name)
#include "clang/Basic/OpenMPKinds.def"
      .Default(OMP_CTX_unknown);
}

llvm::StringRef
clang::getOpenMPContextSelectorName(OpenMPContextSelectorKind Kind) {
  switch (Kind) {
  case OMP_CTX_unknown:
    return "unknown";
#define OPENMP_CONTEXT_SELECTOR(Name)                                          \
  case OMP_CTX_##Name:                                                         \
    return #Name;
#include "clang/Basic/OpenMPKinds.def"
    break;
  }
  llvm_unreachable("Invalid OpenMP context selector kind");
}

OpenMPClauseKind clang::getOpenMPClauseKind(StringRef Str) {
  // 'flush' clause cannot be specified explicitly, because this is an implicit
  // clause for 'flush' directive. If the 'flush' clause is explicitly specified
  // the Parser should generate a warning about extra tokens at the end of the
  // directive.
  if (Str == "flush")
    return OMPC_unknown;
  return llvm::StringSwitch<OpenMPClauseKind>(Str)
#define OPENMP_CLAUSE(Name, Class) .Case(#Name, OMPC_##Name)
#include "clang/Basic/OpenMPKinds.def"
      .Case("uniform", OMPC_uniform)
      .Case("device_type", OMPC_device_type)
      .Case("match", OMPC_match)
      .Default(OMPC_unknown);
}

const char *clang::getOpenMPClauseName(OpenMPClauseKind Kind) {
  assert(Kind <= OMPC_unknown);
  switch (Kind) {
  case OMPC_unknown:
    return "unknown";
#define OPENMP_CLAUSE(Name, Class)                                             \
  case OMPC_##Name:                                                            \
    return #Name;
#include "clang/Basic/OpenMPKinds.def"
  case OMPC_uniform:
    return "uniform";
  case OMPC_threadprivate:
    return "threadprivate or thread local";
  case OMPC_device_type:
    return "device_type";
  case OMPC_match:
    return "match";
  }
  llvm_unreachable("Invalid OpenMP clause kind");
}

unsigned clang::getOpenMPSimpleClauseType(OpenMPClauseKind Kind,
                                          StringRef Str) {
  switch (Kind) {
  case OMPC_default:
    return llvm::StringSwitch<OpenMPDefaultClauseKind>(Str)
#define OPENMP_DEFAULT_KIND(Name) .Case(#Name, OMPC_DEFAULT_##Name)
#include "clang/Basic/OpenMPKinds.def"
        .Default(OMPC_DEFAULT_unknown);
  case OMPC_proc_bind:
    return llvm::StringSwitch<unsigned>(Str)
#define OMP_PROC_BIND_KIND(Enum, Name, Value) .Case(Name, Value)
#include "llvm/Frontend/OpenMP/OMPKinds.def"
        .Default(unsigned(llvm::omp::OMP_PROC_BIND_unknown));
  case OMPC_schedule:
    return llvm::StringSwitch<unsigned>(Str)
#define OPENMP_SCHEDULE_KIND(Name)                                             \
  .Case(#Name, static_cast<unsigned>(OMPC_SCHEDULE_##Name))
#define OPENMP_SCHEDULE_MODIFIER(Name)                                         \
  .Case(#Name, static_cast<unsigned>(OMPC_SCHEDULE_MODIFIER_##Name))
#include "clang/Basic/OpenMPKinds.def"
        .Default(OMPC_SCHEDULE_unknown);
  case OMPC_depend:
    return llvm::StringSwitch<OpenMPDependClauseKind>(Str)
#define OPENMP_DEPEND_KIND(Name) .Case(#Name, OMPC_DEPEND_##Name)
#include "clang/Basic/OpenMPKinds.def"
        .Default(OMPC_DEPEND_unknown);
  case OMPC_linear:
    return llvm::StringSwitch<OpenMPLinearClauseKind>(Str)
#define OPENMP_LINEAR_KIND(Name) .Case(#Name, OMPC_LINEAR_##Name)
#include "clang/Basic/OpenMPKinds.def"
        .Default(OMPC_LINEAR_unknown);
  case OMPC_map:
    return llvm::StringSwitch<unsigned>(Str)
#define OPENMP_MAP_KIND(Name)                                                  \
  .Case(#Name, static_cast<unsigned>(OMPC_MAP_##Name))
#define OPENMP_MAP_MODIFIER_KIND(Name)                                         \
  .Case(#Name, static_cast<unsigned>(OMPC_MAP_MODIFIER_##Name))
#include "clang/Basic/OpenMPKinds.def"
        .Default(OMPC_MAP_unknown);
  case OMPC_to:
    return llvm::StringSwitch<unsigned>(Str)
#define OPENMP_TO_MODIFIER_KIND(Name)                                          \
  .Case(#Name, static_cast<unsigned>(OMPC_TO_MODIFIER_##Name))
#include "clang/Basic/OpenMPKinds.def"
        .Default(OMPC_TO_MODIFIER_unknown);
  case OMPC_from:
    return llvm::StringSwitch<unsigned>(Str)
#define OPENMP_FROM_MODIFIER_KIND(Name)                                     \
  .Case(#Name, static_cast<unsigned>(OMPC_FROM_MODIFIER_##Name))
#include "clang/Basic/OpenMPKinds.def"
        .Default(OMPC_FROM_MODIFIER_unknown);
  case OMPC_dist_schedule:
    return llvm::StringSwitch<OpenMPDistScheduleClauseKind>(Str)
#define OPENMP_DIST_SCHEDULE_KIND(Name) .Case(#Name, OMPC_DIST_SCHEDULE_##Name)
#include "clang/Basic/OpenMPKinds.def"
        .Default(OMPC_DIST_SCHEDULE_unknown);
  case OMPC_defaultmap:
    return llvm::StringSwitch<unsigned>(Str)
#define OPENMP_DEFAULTMAP_KIND(Name)                                           \
  .Case(#Name, static_cast<unsigned>(OMPC_DEFAULTMAP_##Name))
#define OPENMP_DEFAULTMAP_MODIFIER(Name)                                       \
  .Case(#Name, static_cast<unsigned>(OMPC_DEFAULTMAP_MODIFIER_##Name))
#include "clang/Basic/OpenMPKinds.def"
        .Default(OMPC_DEFAULTMAP_unknown);
  case OMPC_atomic_default_mem_order:
     return llvm::StringSwitch<OpenMPAtomicDefaultMemOrderClauseKind>(Str)
#define OPENMP_ATOMIC_DEFAULT_MEM_ORDER_KIND(Name)       \
  .Case(#Name, OMPC_ATOMIC_DEFAULT_MEM_ORDER_##Name)
#include "clang/Basic/OpenMPKinds.def"
        .Default(OMPC_ATOMIC_DEFAULT_MEM_ORDER_unknown);
  case OMPC_device_type:
    return llvm::StringSwitch<OpenMPDeviceType>(Str)
#define OPENMP_DEVICE_TYPE_KIND(Name) .Case(#Name, OMPC_DEVICE_TYPE_##Name)
#include "clang/Basic/OpenMPKinds.def"
        .Default(OMPC_DEVICE_TYPE_unknown);
  case OMPC_lastprivate:
    return llvm::StringSwitch<OpenMPLastprivateModifier>(Str)
#define OPENMP_LASTPRIVATE_KIND(Name) .Case(#Name, OMPC_LASTPRIVATE_##Name)
#include "clang/Basic/OpenMPKinds.def"
        .Default(OMPC_LASTPRIVATE_unknown);
  case OMPC_order:
    return llvm::StringSwitch<OpenMPOrderClauseKind>(Str)
#define OPENMP_ORDER_KIND(Name) .Case(#Name, OMPC_ORDER_##Name)
#include "clang/Basic/OpenMPKinds.def"
        .Default(OMPC_ORDER_unknown);
  case OMPC_unknown:
  case OMPC_threadprivate:
  case OMPC_if:
  case OMPC_final:
  case OMPC_num_threads:
  case OMPC_safelen:
  case OMPC_simdlen:
  case OMPC_allocator:
  case OMPC_allocate:
  case OMPC_collapse:
  case OMPC_private:
  case OMPC_firstprivate:
  case OMPC_shared:
  case OMPC_reduction:
  case OMPC_task_reduction:
  case OMPC_in_reduction:
  case OMPC_aligned:
  case OMPC_copyin:
  case OMPC_copyprivate:
  case OMPC_ordered:
  case OMPC_nowait:
  case OMPC_untied:
  case OMPC_mergeable:
  case OMPC_flush:
  case OMPC_read:
  case OMPC_write:
  case OMPC_update:
  case OMPC_capture:
  case OMPC_seq_cst:
  case OMPC_acq_rel:
  case OMPC_acquire:
  case OMPC_release:
  case OMPC_relaxed:
  case OMPC_device:
  case OMPC_threads:
  case OMPC_simd:
  case OMPC_num_teams:
  case OMPC_thread_limit:
  case OMPC_priority:
  case OMPC_grainsize:
  case OMPC_nogroup:
  case OMPC_num_tasks:
  case OMPC_hint:
  case OMPC_uniform:
  case OMPC_use_device_ptr:
  case OMPC_is_device_ptr:
  case OMPC_unified_address:
  case OMPC_unified_shared_memory:
  case OMPC_reverse_offload:
  case OMPC_dynamic_allocators:
  case OMPC_match:
  case OMPC_nontemporal:
    break;
  }
  llvm_unreachable("Invalid OpenMP simple clause kind");
}

const char *clang::getOpenMPSimpleClauseTypeName(OpenMPClauseKind Kind,
                                                 unsigned Type) {
  switch (Kind) {
  case OMPC_default:
    switch (Type) {
    case OMPC_DEFAULT_unknown:
      return "unknown";
#define OPENMP_DEFAULT_KIND(Name)                                              \
  case OMPC_DEFAULT_##Name:                                                    \
    return #Name;
#include "clang/Basic/OpenMPKinds.def"
    }
    llvm_unreachable("Invalid OpenMP 'default' clause type");
  case OMPC_proc_bind:
    switch (Type) {
#define OMP_PROC_BIND_KIND(Enum, Name, Value)                                  \
  case Value:                                                                  \
    return Name;
#include "llvm/Frontend/OpenMP/OMPKinds.def"
    }
    llvm_unreachable("Invalid OpenMP 'proc_bind' clause type");
  case OMPC_schedule:
    switch (Type) {
    case OMPC_SCHEDULE_unknown:
    case OMPC_SCHEDULE_MODIFIER_last:
      return "unknown";
#define OPENMP_SCHEDULE_KIND(Name)                                             \
    case OMPC_SCHEDULE_##Name:                                                 \
      return #Name;
#define OPENMP_SCHEDULE_MODIFIER(Name)                                         \
    case OMPC_SCHEDULE_MODIFIER_##Name:                                        \
      return #Name;
#include "clang/Basic/OpenMPKinds.def"
    }
    llvm_unreachable("Invalid OpenMP 'schedule' clause type");
  case OMPC_depend:
    switch (Type) {
    case OMPC_DEPEND_unknown:
      return "unknown";
#define OPENMP_DEPEND_KIND(Name)                                             \
  case OMPC_DEPEND_##Name:                                                   \
    return #Name;
#include "clang/Basic/OpenMPKinds.def"
    }
    llvm_unreachable("Invalid OpenMP 'depend' clause type");
  case OMPC_linear:
    switch (Type) {
    case OMPC_LINEAR_unknown:
      return "unknown";
#define OPENMP_LINEAR_KIND(Name)                                             \
  case OMPC_LINEAR_##Name:                                                   \
    return #Name;
#include "clang/Basic/OpenMPKinds.def"
    }
    llvm_unreachable("Invalid OpenMP 'linear' clause type");
  case OMPC_map:
    switch (Type) {
    case OMPC_MAP_unknown:
    case OMPC_MAP_MODIFIER_last:
      return "unknown";
#define OPENMP_MAP_KIND(Name)                                                \
  case OMPC_MAP_##Name:                                                      \
    return #Name;
#define OPENMP_MAP_MODIFIER_KIND(Name)                                       \
  case OMPC_MAP_MODIFIER_##Name:                                             \
    return #Name;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    llvm_unreachable("Invalid OpenMP 'map' clause type");
  case OMPC_to:
    switch (Type) {
    case OMPC_TO_MODIFIER_unknown:
      return "unknown";
#define OPENMP_TO_MODIFIER_KIND(Name)                                          \
  case OMPC_TO_MODIFIER_##Name:                                                \
    return #Name;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    llvm_unreachable("Invalid OpenMP 'to' clause type");
  case OMPC_from:
    switch (Type) {
    case OMPC_FROM_MODIFIER_unknown:
      return "unknown";
#define OPENMP_FROM_MODIFIER_KIND(Name)                                        \
  case OMPC_FROM_MODIFIER_##Name:                                              \
    return #Name;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    llvm_unreachable("Invalid OpenMP 'from' clause type");
  case OMPC_dist_schedule:
    switch (Type) {
    case OMPC_DIST_SCHEDULE_unknown:
      return "unknown";
#define OPENMP_DIST_SCHEDULE_KIND(Name)                                      \
  case OMPC_DIST_SCHEDULE_##Name:                                            \
    return #Name;
#include "clang/Basic/OpenMPKinds.def"
    }
    llvm_unreachable("Invalid OpenMP 'dist_schedule' clause type");
  case OMPC_defaultmap:
    switch (Type) {
    case OMPC_DEFAULTMAP_unknown:
    case OMPC_DEFAULTMAP_MODIFIER_last:
      return "unknown";
#define OPENMP_DEFAULTMAP_KIND(Name)                                         \
    case OMPC_DEFAULTMAP_##Name:                                             \
      return #Name;
#define OPENMP_DEFAULTMAP_MODIFIER(Name)                                     \
    case OMPC_DEFAULTMAP_MODIFIER_##Name:                                    \
      return #Name;
#include "clang/Basic/OpenMPKinds.def"
    }
    llvm_unreachable("Invalid OpenMP 'schedule' clause type");
  case OMPC_atomic_default_mem_order:
    switch (Type) {
    case OMPC_ATOMIC_DEFAULT_MEM_ORDER_unknown:
      return "unknown";
#define OPENMP_ATOMIC_DEFAULT_MEM_ORDER_KIND(Name)                           \
    case OMPC_ATOMIC_DEFAULT_MEM_ORDER_##Name:                               \
      return #Name;
#include "clang/Basic/OpenMPKinds.def"
}
    llvm_unreachable("Invalid OpenMP 'atomic_default_mem_order' clause type");
  case OMPC_device_type:
    switch (Type) {
    case OMPC_DEVICE_TYPE_unknown:
      return "unknown";
#define OPENMP_DEVICE_TYPE_KIND(Name)                                          \
    case OMPC_DEVICE_TYPE_##Name:                                              \
      return #Name;
#include "clang/Basic/OpenMPKinds.def"
    }
    llvm_unreachable("Invalid OpenMP 'device_type' clause type");
  case OMPC_lastprivate:
    switch (Type) {
    case OMPC_LASTPRIVATE_unknown:
      return "unknown";
#define OPENMP_LASTPRIVATE_KIND(Name)                                          \
    case OMPC_LASTPRIVATE_##Name:                                              \
      return #Name;
#include "clang/Basic/OpenMPKinds.def"
    }
    llvm_unreachable("Invalid OpenMP 'lastprivate' clause type");
  case OMPC_order:
    switch (Type) {
    case OMPC_ORDER_unknown:
      return "unknown";
#define OPENMP_ORDER_KIND(Name)                                                \
    case OMPC_ORDER_##Name:                                                    \
      return #Name;
#include "clang/Basic/OpenMPKinds.def"
    }
    llvm_unreachable("Invalid OpenMP 'order' clause type");
  case OMPC_unknown:
  case OMPC_threadprivate:
  case OMPC_if:
  case OMPC_final:
  case OMPC_num_threads:
  case OMPC_safelen:
  case OMPC_simdlen:
  case OMPC_allocator:
  case OMPC_allocate:
  case OMPC_collapse:
  case OMPC_private:
  case OMPC_firstprivate:
  case OMPC_shared:
  case OMPC_reduction:
  case OMPC_task_reduction:
  case OMPC_in_reduction:
  case OMPC_aligned:
  case OMPC_copyin:
  case OMPC_copyprivate:
  case OMPC_ordered:
  case OMPC_nowait:
  case OMPC_untied:
  case OMPC_mergeable:
  case OMPC_flush:
  case OMPC_read:
  case OMPC_write:
  case OMPC_update:
  case OMPC_capture:
  case OMPC_seq_cst:
  case OMPC_acq_rel:
  case OMPC_acquire:
  case OMPC_release:
  case OMPC_relaxed:
  case OMPC_device:
  case OMPC_threads:
  case OMPC_simd:
  case OMPC_num_teams:
  case OMPC_thread_limit:
  case OMPC_priority:
  case OMPC_grainsize:
  case OMPC_nogroup:
  case OMPC_num_tasks:
  case OMPC_hint:
  case OMPC_uniform:
  case OMPC_use_device_ptr:
  case OMPC_is_device_ptr:
  case OMPC_unified_address:
  case OMPC_unified_shared_memory:
  case OMPC_reverse_offload:
  case OMPC_dynamic_allocators:
  case OMPC_match:
  case OMPC_nontemporal:
    break;
  }
  llvm_unreachable("Invalid OpenMP simple clause kind");
}

bool clang::isAllowedClauseForDirective(OpenMPDirectiveKind DKind,
                                        OpenMPClauseKind CKind,
                                        unsigned OpenMPVersion) {
  assert(unsigned(DKind) <= unsigned(OMPD_unknown));
  assert(CKind <= OMPC_unknown);
  // Nontemporal clause is not supported in OpenMP < 5.0.
  if (OpenMPVersion < 50 && CKind == OMPC_nontemporal)
    return false;
  // Order clause is not supported in OpenMP < 5.0.
  if (OpenMPVersion < 50 && CKind == OMPC_order)
    return false;
  switch (DKind) {
  case OMPD_parallel:
    switch (CKind) {
#define OPENMP_PARALLEL_CLAUSE(Name)                                           \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_simd:
    if (OpenMPVersion < 50 && CKind == OMPC_if)
      return false;
    switch (CKind) {
#define OPENMP_SIMD_CLAUSE(Name)                                               \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_for:
    switch (CKind) {
#define OPENMP_FOR_CLAUSE(Name)                                                \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_for_simd:
    if (OpenMPVersion < 50 && CKind == OMPC_if)
      return false;
    switch (CKind) {
#define OPENMP_FOR_SIMD_CLAUSE(Name)                                           \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_sections:
    switch (CKind) {
#define OPENMP_SECTIONS_CLAUSE(Name)                                           \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_single:
    switch (CKind) {
#define OPENMP_SINGLE_CLAUSE(Name)                                             \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_parallel_for:
    switch (CKind) {
#define OPENMP_PARALLEL_FOR_CLAUSE(Name)                                       \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_parallel_for_simd:
    switch (CKind) {
#define OPENMP_PARALLEL_FOR_SIMD_CLAUSE(Name)                                  \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_parallel_master:
    switch (CKind) {
#define OPENMP_PARALLEL_MASTER_CLAUSE(Name)                                    \
    case OMPC_##Name:                                                          \
      return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_parallel_sections:
    switch (CKind) {
#define OPENMP_PARALLEL_SECTIONS_CLAUSE(Name)                                  \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_task:
    switch (CKind) {
#define OPENMP_TASK_CLAUSE(Name)                                               \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_flush:
    if (CKind == OMPC_flush)
      return true;
    if (OpenMPVersion < 50)
      return false;
    switch (CKind) {
#define OPENMP_FLUSH_CLAUSE(Name)                                              \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_atomic:
    if (OpenMPVersion < 50 && (CKind == OMPC_acq_rel || CKind == OMPC_acquire ||
                               CKind == OMPC_release || CKind == OMPC_relaxed))
      return false;
    switch (CKind) {
#define OPENMP_ATOMIC_CLAUSE(Name)                                             \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_target:
    switch (CKind) {
#define OPENMP_TARGET_CLAUSE(Name)                                             \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_requires:
    switch (CKind) {
#define OPENMP_REQUIRES_CLAUSE(Name)                                             \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_target_data:
    switch (CKind) {
#define OPENMP_TARGET_DATA_CLAUSE(Name)                                        \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_target_enter_data:
    switch (CKind) {
#define OPENMP_TARGET_ENTER_DATA_CLAUSE(Name)                                  \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_target_exit_data:
    switch (CKind) {
#define OPENMP_TARGET_EXIT_DATA_CLAUSE(Name)                                   \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_target_parallel:
    switch (CKind) {
#define OPENMP_TARGET_PARALLEL_CLAUSE(Name)                                    \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_target_parallel_for:
    switch (CKind) {
#define OPENMP_TARGET_PARALLEL_FOR_CLAUSE(Name)                                \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_target_update:
    switch (CKind) {
#define OPENMP_TARGET_UPDATE_CLAUSE(Name)                                      \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_teams:
    switch (CKind) {
#define OPENMP_TEAMS_CLAUSE(Name)                                              \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_cancel:
    switch (CKind) {
#define OPENMP_CANCEL_CLAUSE(Name)                                             \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_ordered:
    switch (CKind) {
#define OPENMP_ORDERED_CLAUSE(Name)                                            \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_taskloop:
    switch (CKind) {
#define OPENMP_TASKLOOP_CLAUSE(Name)                                           \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_taskloop_simd:
    switch (CKind) {
#define OPENMP_TASKLOOP_SIMD_CLAUSE(Name)                                      \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_master_taskloop:
    switch (CKind) {
#define OPENMP_MASTER_TASKLOOP_CLAUSE(Name)                                    \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_master_taskloop_simd:
    switch (CKind) {
#define OPENMP_MASTER_TASKLOOP_SIMD_CLAUSE(Name)                               \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_parallel_master_taskloop:
    switch (CKind) {
#define OPENMP_PARALLEL_MASTER_TASKLOOP_CLAUSE(Name)                           \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_parallel_master_taskloop_simd:
    switch (CKind) {
#define OPENMP_PARALLEL_MASTER_TASKLOOP_SIMD_CLAUSE(Name)                      \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_critical:
    switch (CKind) {
#define OPENMP_CRITICAL_CLAUSE(Name)                                           \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_distribute:
    switch (CKind) {
#define OPENMP_DISTRIBUTE_CLAUSE(Name)                                         \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_distribute_parallel_for:
    switch (CKind) {
#define OPENMP_DISTRIBUTE_PARALLEL_FOR_CLAUSE(Name)                            \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_distribute_parallel_for_simd:
    switch (CKind) {
#define OPENMP_DISTRIBUTE_PARALLEL_FOR_SIMD_CLAUSE(Name)                       \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_distribute_simd:
    if (OpenMPVersion < 50 && CKind == OMPC_if)
      return false;
    switch (CKind) {
#define OPENMP_DISTRIBUTE_SIMD_CLAUSE(Name)                                    \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_target_parallel_for_simd:
    switch (CKind) {
#define OPENMP_TARGET_PARALLEL_FOR_SIMD_CLAUSE(Name)                           \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_target_simd:
    switch (CKind) {
#define OPENMP_TARGET_SIMD_CLAUSE(Name)                                        \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_teams_distribute:
    switch (CKind) {
#define OPENMP_TEAMS_DISTRIBUTE_CLAUSE(Name)                                   \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_teams_distribute_simd:
    if (OpenMPVersion < 50 && CKind == OMPC_if)
      return false;
    switch (CKind) {
#define OPENMP_TEAMS_DISTRIBUTE_SIMD_CLAUSE(Name)                              \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_teams_distribute_parallel_for_simd:
    switch (CKind) {
#define OPENMP_TEAMS_DISTRIBUTE_PARALLEL_FOR_SIMD_CLAUSE(Name)                 \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_teams_distribute_parallel_for:
    switch (CKind) {
#define OPENMP_TEAMS_DISTRIBUTE_PARALLEL_FOR_CLAUSE(Name)                      \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_target_teams:
    switch (CKind) {
#define OPENMP_TARGET_TEAMS_CLAUSE(Name)                                       \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_target_teams_distribute:
    switch (CKind) {
#define OPENMP_TARGET_TEAMS_DISTRIBUTE_CLAUSE(Name)                            \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_target_teams_distribute_parallel_for:
    switch (CKind) {
#define OPENMP_TARGET_TEAMS_DISTRIBUTE_PARALLEL_FOR_CLAUSE(Name)               \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_target_teams_distribute_parallel_for_simd:
    switch (CKind) {
#define OPENMP_TARGET_TEAMS_DISTRIBUTE_PARALLEL_FOR_SIMD_CLAUSE(Name)          \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_target_teams_distribute_simd:
    switch (CKind) {
#define OPENMP_TARGET_TEAMS_DISTRIBUTE_SIMD_CLAUSE(Name)                       \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_taskgroup:
    switch (CKind) {
#define OPENMP_TASKGROUP_CLAUSE(Name)                                          \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_declare_mapper:
    switch (CKind) {
#define OPENMP_DECLARE_MAPPER_CLAUSE(Name)                                     \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_allocate:
    switch (CKind) {
#define OPENMP_ALLOCATE_CLAUSE(Name)                                           \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_declare_variant:
    switch (CKind) {
#define OPENMP_DECLARE_VARIANT_CLAUSE(Name)                                    \
  case OMPC_##Name:                                                            \
    return true;
#include "clang/Basic/OpenMPKinds.def"
    default:
      break;
    }
    break;
  case OMPD_declare_target:
  case OMPD_end_declare_target:
  case OMPD_unknown:
  case OMPD_threadprivate:
  case OMPD_section:
  case OMPD_master:
  case OMPD_taskyield:
  case OMPD_barrier:
  case OMPD_taskwait:
  case OMPD_cancellation_point:
  case OMPD_declare_reduction:
  case OMPD_declare_simd:
    break;
  }
  return false;
}

bool clang::isOpenMPLoopDirective(OpenMPDirectiveKind DKind) {
  return DKind == OMPD_simd || DKind == OMPD_for || DKind == OMPD_for_simd ||
         DKind == OMPD_parallel_for || DKind == OMPD_parallel_for_simd ||
         DKind == OMPD_taskloop || DKind == OMPD_taskloop_simd ||
         DKind == OMPD_master_taskloop || DKind == OMPD_master_taskloop_simd ||
         DKind == OMPD_parallel_master_taskloop ||
         DKind == OMPD_parallel_master_taskloop_simd ||
         DKind == OMPD_distribute || DKind == OMPD_target_parallel_for ||
         DKind == OMPD_distribute_parallel_for ||
         DKind == OMPD_distribute_parallel_for_simd ||
         DKind == OMPD_distribute_simd ||
         DKind == OMPD_target_parallel_for_simd || DKind == OMPD_target_simd ||
         DKind == OMPD_teams_distribute ||
         DKind == OMPD_teams_distribute_simd ||
         DKind == OMPD_teams_distribute_parallel_for_simd ||
         DKind == OMPD_teams_distribute_parallel_for ||
         DKind == OMPD_target_teams_distribute ||
         DKind == OMPD_target_teams_distribute_parallel_for ||
         DKind == OMPD_target_teams_distribute_parallel_for_simd ||
         DKind == OMPD_target_teams_distribute_simd;
}

bool clang::isOpenMPWorksharingDirective(OpenMPDirectiveKind DKind) {
  return DKind == OMPD_for || DKind == OMPD_for_simd ||
         DKind == OMPD_sections || DKind == OMPD_section ||
         DKind == OMPD_single || DKind == OMPD_parallel_for ||
         DKind == OMPD_parallel_for_simd || DKind == OMPD_parallel_sections ||
         DKind == OMPD_target_parallel_for ||
         DKind == OMPD_distribute_parallel_for ||
         DKind == OMPD_distribute_parallel_for_simd ||
         DKind == OMPD_target_parallel_for_simd ||
         DKind == OMPD_teams_distribute_parallel_for_simd ||
         DKind == OMPD_teams_distribute_parallel_for ||
         DKind == OMPD_target_teams_distribute_parallel_for ||
         DKind == OMPD_target_teams_distribute_parallel_for_simd;
}

bool clang::isOpenMPTaskLoopDirective(OpenMPDirectiveKind DKind) {
  return DKind == OMPD_taskloop || DKind == OMPD_taskloop_simd ||
         DKind == OMPD_master_taskloop || DKind == OMPD_master_taskloop_simd ||
         DKind == OMPD_parallel_master_taskloop ||
         DKind == OMPD_parallel_master_taskloop_simd;
}

bool clang::isOpenMPParallelDirective(OpenMPDirectiveKind DKind) {
  return DKind == OMPD_parallel || DKind == OMPD_parallel_for ||
         DKind == OMPD_parallel_for_simd || DKind == OMPD_parallel_sections ||
         DKind == OMPD_target_parallel || DKind == OMPD_target_parallel_for ||
         DKind == OMPD_distribute_parallel_for ||
         DKind == OMPD_distribute_parallel_for_simd ||
         DKind == OMPD_target_parallel_for_simd ||
         DKind == OMPD_teams_distribute_parallel_for ||
         DKind == OMPD_teams_distribute_parallel_for_simd ||
         DKind == OMPD_target_teams_distribute_parallel_for ||
         DKind == OMPD_target_teams_distribute_parallel_for_simd ||
         DKind == OMPD_parallel_master ||
         DKind == OMPD_parallel_master_taskloop ||
         DKind == OMPD_parallel_master_taskloop_simd;
}

bool clang::isOpenMPTargetExecutionDirective(OpenMPDirectiveKind DKind) {
  return DKind == OMPD_target || DKind == OMPD_target_parallel ||
         DKind == OMPD_target_parallel_for ||
         DKind == OMPD_target_parallel_for_simd || DKind == OMPD_target_simd ||
         DKind == OMPD_target_teams || DKind == OMPD_target_teams_distribute ||
         DKind == OMPD_target_teams_distribute_parallel_for ||
         DKind == OMPD_target_teams_distribute_parallel_for_simd ||
         DKind == OMPD_target_teams_distribute_simd;
}

bool clang::isOpenMPTargetDataManagementDirective(OpenMPDirectiveKind DKind) {
  return DKind == OMPD_target_data || DKind == OMPD_target_enter_data ||
         DKind == OMPD_target_exit_data || DKind == OMPD_target_update;
}

bool clang::isOpenMPNestingTeamsDirective(OpenMPDirectiveKind DKind) {
  return DKind == OMPD_teams || DKind == OMPD_teams_distribute ||
         DKind == OMPD_teams_distribute_simd ||
         DKind == OMPD_teams_distribute_parallel_for_simd ||
         DKind == OMPD_teams_distribute_parallel_for;
}

bool clang::isOpenMPTeamsDirective(OpenMPDirectiveKind DKind) {
  return isOpenMPNestingTeamsDirective(DKind) ||
         DKind == OMPD_target_teams || DKind == OMPD_target_teams_distribute ||
         DKind == OMPD_target_teams_distribute_parallel_for ||
         DKind == OMPD_target_teams_distribute_parallel_for_simd ||
         DKind == OMPD_target_teams_distribute_simd;
}

bool clang::isOpenMPSimdDirective(OpenMPDirectiveKind DKind) {
  return DKind == OMPD_simd || DKind == OMPD_for_simd ||
         DKind == OMPD_parallel_for_simd || DKind == OMPD_taskloop_simd ||
         DKind == OMPD_master_taskloop_simd ||
         DKind == OMPD_parallel_master_taskloop_simd ||
         DKind == OMPD_distribute_parallel_for_simd ||
         DKind == OMPD_distribute_simd || DKind == OMPD_target_simd ||
         DKind == OMPD_teams_distribute_simd ||
         DKind == OMPD_teams_distribute_parallel_for_simd ||
         DKind == OMPD_target_teams_distribute_parallel_for_simd ||
         DKind == OMPD_target_teams_distribute_simd ||
         DKind == OMPD_target_parallel_for_simd;
}

bool clang::isOpenMPNestingDistributeDirective(OpenMPDirectiveKind Kind) {
  return Kind == OMPD_distribute || Kind == OMPD_distribute_parallel_for ||
         Kind == OMPD_distribute_parallel_for_simd ||
         Kind == OMPD_distribute_simd;
  // TODO add next directives.
}

bool clang::isOpenMPDistributeDirective(OpenMPDirectiveKind Kind) {
  return isOpenMPNestingDistributeDirective(Kind) ||
         Kind == OMPD_teams_distribute || Kind == OMPD_teams_distribute_simd ||
         Kind == OMPD_teams_distribute_parallel_for_simd ||
         Kind == OMPD_teams_distribute_parallel_for ||
         Kind == OMPD_target_teams_distribute ||
         Kind == OMPD_target_teams_distribute_parallel_for ||
         Kind == OMPD_target_teams_distribute_parallel_for_simd ||
         Kind == OMPD_target_teams_distribute_simd;
}

bool clang::isOpenMPPrivate(OpenMPClauseKind Kind) {
  return Kind == OMPC_private || Kind == OMPC_firstprivate ||
         Kind == OMPC_lastprivate || Kind == OMPC_linear ||
         Kind == OMPC_reduction || Kind == OMPC_task_reduction ||
         Kind == OMPC_in_reduction; // TODO add next clauses like 'reduction'.
}

bool clang::isOpenMPThreadPrivate(OpenMPClauseKind Kind) {
  return Kind == OMPC_threadprivate || Kind == OMPC_copyin;
}

bool clang::isOpenMPTaskingDirective(OpenMPDirectiveKind Kind) {
  return Kind == OMPD_task || isOpenMPTaskLoopDirective(Kind);
}

bool clang::isOpenMPLoopBoundSharingDirective(OpenMPDirectiveKind Kind) {
  return Kind == OMPD_distribute_parallel_for ||
         Kind == OMPD_distribute_parallel_for_simd ||
         Kind == OMPD_teams_distribute_parallel_for_simd ||
         Kind == OMPD_teams_distribute_parallel_for ||
         Kind == OMPD_target_teams_distribute_parallel_for ||
         Kind == OMPD_target_teams_distribute_parallel_for_simd;
}

void clang::getOpenMPCaptureRegions(
    SmallVectorImpl<OpenMPDirectiveKind> &CaptureRegions,
    OpenMPDirectiveKind DKind) {
  assert(DKind <= OMPD_unknown);
  switch (DKind) {
  case OMPD_parallel:
  case OMPD_parallel_for:
  case OMPD_parallel_for_simd:
  case OMPD_parallel_master:
  case OMPD_parallel_sections:
  case OMPD_distribute_parallel_for:
  case OMPD_distribute_parallel_for_simd:
    CaptureRegions.push_back(OMPD_parallel);
    break;
  case OMPD_target_teams:
  case OMPD_target_teams_distribute:
  case OMPD_target_teams_distribute_simd:
    CaptureRegions.push_back(OMPD_task);
    CaptureRegions.push_back(OMPD_target);
    CaptureRegions.push_back(OMPD_teams);
    break;
  case OMPD_teams:
  case OMPD_teams_distribute:
  case OMPD_teams_distribute_simd:
    CaptureRegions.push_back(OMPD_teams);
    break;
  case OMPD_target:
  case OMPD_target_simd:
    CaptureRegions.push_back(OMPD_task);
    CaptureRegions.push_back(OMPD_target);
    break;
  case OMPD_teams_distribute_parallel_for:
  case OMPD_teams_distribute_parallel_for_simd:
    CaptureRegions.push_back(OMPD_teams);
    CaptureRegions.push_back(OMPD_parallel);
    break;
  case OMPD_target_parallel:
  case OMPD_target_parallel_for:
  case OMPD_target_parallel_for_simd:
    CaptureRegions.push_back(OMPD_task);
    CaptureRegions.push_back(OMPD_target);
    CaptureRegions.push_back(OMPD_parallel);
    break;
  case OMPD_task:
  case OMPD_target_enter_data:
  case OMPD_target_exit_data:
  case OMPD_target_update:
    CaptureRegions.push_back(OMPD_task);
    break;
  case OMPD_taskloop:
  case OMPD_taskloop_simd:
  case OMPD_master_taskloop:
  case OMPD_master_taskloop_simd:
    CaptureRegions.push_back(OMPD_taskloop);
    break;
  case OMPD_parallel_master_taskloop:
  case OMPD_parallel_master_taskloop_simd:
    CaptureRegions.push_back(OMPD_parallel);
    CaptureRegions.push_back(OMPD_taskloop);
    break;
  case OMPD_target_teams_distribute_parallel_for:
  case OMPD_target_teams_distribute_parallel_for_simd:
    CaptureRegions.push_back(OMPD_task);
    CaptureRegions.push_back(OMPD_target);
    CaptureRegions.push_back(OMPD_teams);
    CaptureRegions.push_back(OMPD_parallel);
    break;
  case OMPD_simd:
  case OMPD_for:
  case OMPD_for_simd:
  case OMPD_sections:
  case OMPD_section:
  case OMPD_single:
  case OMPD_master:
  case OMPD_critical:
  case OMPD_taskgroup:
  case OMPD_distribute:
  case OMPD_ordered:
  case OMPD_atomic:
  case OMPD_target_data:
  case OMPD_distribute_simd:
    CaptureRegions.push_back(OMPD_unknown);
    break;
  case OMPD_threadprivate:
  case OMPD_allocate:
  case OMPD_taskyield:
  case OMPD_barrier:
  case OMPD_taskwait:
  case OMPD_cancellation_point:
  case OMPD_cancel:
  case OMPD_flush:
  case OMPD_declare_reduction:
  case OMPD_declare_mapper:
  case OMPD_declare_simd:
  case OMPD_declare_target:
  case OMPD_end_declare_target:
  case OMPD_requires:
  case OMPD_declare_variant:
    llvm_unreachable("OpenMP Directive is not allowed");
  case OMPD_unknown:
    llvm_unreachable("Unknown OpenMP directive");
  }
}
