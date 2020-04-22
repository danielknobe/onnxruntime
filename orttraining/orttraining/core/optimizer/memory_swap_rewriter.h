// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "core/optimizer/rewrite_rule.h"

namespace onnxruntime {

/**
@Class MemorySwapRewriter

Rewrite rule for adding memory swap nodes.
*/
class MemorySwapRewriter : public RewriteRule {
 public:
  MemorySwapRewriter(int min_topo_distance) noexcept
      : RewriteRule("MemorySwap"),
        min_topo_distance_(min_topo_distance) {
  }

  std::vector<std::string> TargetOpTypes() const noexcept override {
    return {};  // enable for all nodes
  }

 private:
  bool SatisfyCondition(const Graph& graph, const Node& node, const logging::Logger& logger) const override;

  Status Apply(Graph& graph, Node& node, RewriteRuleEffect& rule_effect, const logging::Logger& logger) const override;

  int min_topo_distance_;
};

/**
@Class MemorySwapControlEdgesRewriter

Rewrite rule for restoring control edges for memory swap nodes after loading.
*/
class AddControlEdgeForMemorySwapRewriter : public RewriteRule {
 public:
  AddControlEdgeForMemorySwapRewriter() noexcept
      : RewriteRule("MemorySwapControlEdgeRestore") {
  }

  std::vector<std::string> TargetOpTypes() const noexcept override {
    return {"SwapToHost"};
  }

 private:
  bool SatisfyCondition(const Graph& /*graph*/, const Node& /*node*/, const logging::Logger& /*logger*/) const override {
    return true;
  }

  Status Apply(Graph& graph, Node& node, RewriteRuleEffect& rule_effect, const logging::Logger& logger) const override;

  void AddEdgeInForward(Graph& graph, Node& node, const std::unordered_map<NodeIndex, int>& topo_indices) const;
};

}  // namespace onnxruntime
