#include "AVLRepairInt.hpp"
#include "test_framework.hpp"
#include "test_helpers.hpp"

#include <iostream>
#include <optional>
#include <vector>

struct TestCase {
    const char* name;
    void (*func)();
};

static void test_load_recompute_and_inorder() {
    AVLRepairInt tree;
    tree.loadFromLevelOrder({20, 10, 30, 5, 15, 25, 35});

    expectEq(tree.recomputeHeights(), 3, "balanced tree root height");
    expectTrue(tree.isAVL(), "balanced tree should already satisfy AVL");
    expectIntVectorEq(tree.inorderKeys(), {5, 10, 15, 20, 25, 30, 35}, "balanced tree inorder");
}

static void test_analyze_balanced_tree_none() {
    AVLRepairInt tree;
    tree.loadFromLevelOrder({20, 10, 30});

    RepairStep step = tree.analyzeNextRepair();
    expectFalse(step.foundBadNode, "balanced tree should not report a bad node");
    expectCaseEq(step.rotationCase, RotationCase::None, "balanced tree case");
}

static void test_analyze_ll_case() {
    AVLRepairInt tree;
    tree.loadFromLevelOrder({30, 20, std::nullopt, 10});

    RepairStep step = tree.analyzeNextRepair();
    expectTrue(step.foundBadNode, "LL tree should report a bad node");
    expectEq(step.badKey, 30, "LL bad key");
    expectEq(step.badBF, 2, "LL bad bf");
    expectEq(step.childKey, 20, "LL child key");
    expectEq(step.childBF, 1, "LL child bf");
    expectCaseEq(step.rotationCase, RotationCase::LL, "LL case name");
    expectEq(step.newSubtreeRootKey, 20, "LL predicted new root");
}

static void test_repair_ll_case() {
    AVLRepairInt tree;
    tree.loadFromLevelOrder({30, 20, std::nullopt, 10});

    RepairStep step{};
    expectTrue(tree.repairNextImbalance(step), "repair LL should succeed");
    expectCaseEq(step.rotationCase, RotationCase::LL, "repair LL case");
    expectOptionalVectorEq(
        tree.levelOrderWithNulls(),
        {20, 10, 30},
        "LL repaired tree shape"
    );
    expectTrue(tree.isAVL(), "LL repaired tree should be AVL");
}

static void test_repair_rr_case() {
    AVLRepairInt tree;
    tree.loadFromLevelOrder({10, std::nullopt, 20, std::nullopt, std::nullopt, std::nullopt, 30});

    RepairStep step{};
    expectTrue(tree.repairNextImbalance(step), "repair RR should succeed");
    expectCaseEq(step.rotationCase, RotationCase::RR, "repair RR case");
    expectOptionalVectorEq(
        tree.levelOrderWithNulls(),
        {20, 10, 30},
        "RR repaired tree shape"
    );
}

static void test_repair_lr_case() {
    AVLRepairInt tree;
    tree.loadFromLevelOrder({30, 10, std::nullopt, std::nullopt, 20});

    RepairStep step{};
    expectTrue(tree.repairNextImbalance(step), "repair LR should succeed");
    expectCaseEq(step.rotationCase, RotationCase::LR, "repair LR case");
    expectOptionalVectorEq(
        tree.levelOrderWithNulls(),
        {20, 10, 30},
        "LR repaired tree shape"
    );
}

static void test_repair_rl_case() {
    AVLRepairInt tree;
    tree.loadFromLevelOrder({10, std::nullopt, 30, std::nullopt, std::nullopt, 20});

    RepairStep step{};
    expectTrue(tree.repairNextImbalance(step), "repair RL should succeed");
    expectCaseEq(step.rotationCase, RotationCase::RL, "repair RL case");
    expectOptionalVectorEq(
        tree.levelOrderWithNulls(),
        {20, 10, 30},
        "RL repaired tree shape"
    );
}

static void test_analyze_r0_delete_case() {
    AVLRepairInt tree;
    tree.loadFromLevelOrder({50, std::nullopt, 70, std::nullopt, std::nullopt, 60, 80});

    RepairStep step = tree.analyzeNextRepair();
    expectTrue(step.foundBadNode, "R0 tree should report a bad node");
    expectEq(step.badKey, 50, "R0 bad key");
    expectEq(step.badBF, -2, "R0 bad bf");
    expectEq(step.childKey, 70, "R0 child key");
    expectEq(step.childBF, 0, "R0 child bf");
    expectCaseEq(step.rotationCase, RotationCase::R0, "R0 case name");
    expectEq(step.newSubtreeRootKey, 70, "R0 predicted new root");
}

static void test_repair_l0_delete_case() {
    AVLRepairInt tree;
    tree.loadFromLevelOrder({50, 30, std::nullopt, 20, 40});

    RepairStep step{};
    expectTrue(tree.repairNextImbalance(step), "repair L0 should succeed");
    expectCaseEq(step.rotationCase, RotationCase::L0, "repair L0 case");
    expectOptionalVectorEq(
        tree.levelOrderWithNulls(),
        {30, 20, 50, std::nullopt, std::nullopt, 40},
        "L0 repaired tree shape"
    );
    expectTrue(tree.isAVL(), "L0 repaired tree should be AVL");
}

static void test_repair_until_avl_one_step() {
    AVLRepairInt tree;
    tree.loadFromLevelOrder({30, 20, std::nullopt, 10});

    std::vector<RepairStep> steps = tree.repairUntilAVL();
    expectEq(static_cast<int>(steps.size()), 1, "one-step repairUntilAVL count");
    expectCaseEq(steps[0].rotationCase, RotationCase::LL, "one-step repairUntilAVL case");
    expectOptionalVectorEq(tree.levelOrderWithNulls(), {20, 10, 30}, "one-step repairUntilAVL shape");
}

static std::vector<TestCase> makePublicTests() {
    return {
        {"load_recompute_and_inorder", test_load_recompute_and_inorder},
        {"analyze_balanced_tree_none", test_analyze_balanced_tree_none},
        {"analyze_ll_case", test_analyze_ll_case},
        {"repair_ll_case", test_repair_ll_case},
        {"repair_rr_case", test_repair_rr_case},
        {"repair_lr_case", test_repair_lr_case},
        {"repair_rl_case", test_repair_rl_case},
        {"analyze_r0_delete_case", test_analyze_r0_delete_case},
        {"repair_l0_delete_case", test_repair_l0_delete_case},
        {"repair_until_avl_one_step", test_repair_until_avl_one_step}
    };
}

int main() {
    std::vector<TestCase> tests = makePublicTests();
    int passed = 0;

    for (const TestCase& test : tests) {
        try {
            test.func();
            ++passed;
            std::cout << "[PASS] " << test.name << "\n";
        } catch (const std::exception& ex) {
            std::cout << "[FAIL] " << test.name << " -- " << ex.what() << "\n";
        } catch (...) {
            std::cout << "[FAIL] " << test.name << " -- unknown exception\n";
        }
    }

    std::cout << "Passed " << passed << " / " << static_cast<int>(tests.size()) << " public tests\n";
    return passed == static_cast<int>(tests.size()) ? 0 : 1;
}
