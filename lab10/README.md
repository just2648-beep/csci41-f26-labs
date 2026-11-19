CSCI 41 - Lab 10 - AVL Rebalance Trace

Top-level goal
Build an AVLRepairInt ADT that can analyze and repair a BST that is already in a post-insert or post-delete, pre-rebalance state.

What this lab is about
- recomputing node heights
- reading balance factors
- finding the deepest bad node
- classifying AVL cases:
  - LL
  - LR
  - RR
  - RL
  - L0
  - R0
- performing one local repair
- repeating repairs until the whole tree is AVL-balanced again

Important rules
- Use the provided AVLNode structure.
- Unique keys only.
- Use this height convention:
  - height(nullptr) = 0
  - a leaf has height 1
- Use this balance-factor convention:
  - bf(node) = height(left) - height(right)
- Do not replace the tree with std::set, std::map, std::vector-based fake trees, or other library containers.
- Match the required behavior exactly. Grading is based on tests.

Files you should edit
- AVLRepairInt.cpp

Files you should not edit
- AVLRepairInt.hpp
- tests/public_tests.cpp
- tests/test_framework.hpp
- tests/test_helpers.hpp
- CMakeLists.txt
- main_demo.cpp

Required behavior summary
- loadFromLevelOrder(...) builds the starting tree for you.
- recomputeHeights() updates stored heights and returns the root height.
- isAVL() returns true only when every node is balanced.
- analyzeNextRepair() reports the deepest bad node and the repair that should happen next.
- repairNextImbalance(...) repairs exactly one deepest imbalance.
- repairUntilAVL() keeps repairing until the whole tree is AVL again.

Suggested work order
1. Implement heightOf, updateHeight, and recomputeHeights.
2. Implement balanceFactor and isAVL.
3. Implement rotateLeft and rotateRight.
4. Implement findDeepestImbalance.
5. Implement classifyCase.
6. Implement analyzeNextRepair.
7. Implement repairNextImbalance.
8. Implement repairUntilAVL.
9. Re-run the public tests.

Starting nudge
- Draw the tree and write heights bottom-up before you write any case names.
- For insertion-style trees, the deepest bad node is the first bad node from the new leaf upward.
- For deletion-style trees, child balance factor 0 is possible and still means a single rotation.

Public/private grading model
- 10 public tests are included.
- The TA bundle is expected to use additional private tests.
