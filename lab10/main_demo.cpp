#include "AVLRepairInt.hpp"

#include <iostream>
#include <optional>
#include <vector>

int main() {
    AVLRepairInt tree;
    tree.loadFromLevelOrder(std::vector<std::optional<int>>{
        30, 20, std::nullopt, 10
    });

    RepairStep step = tree.analyzeNextRepair();
    std::cout << "Found bad node? " << (step.foundBadNode ? "yes" : "no") << "\n";
    std::cout << "Case: " << rotationCaseToString(step.rotationCase) << "\n";
    std::cout << "Predicted new subtree root: " << step.newSubtreeRootKey << "\n";

    if (tree.repairNextImbalance(step)) {
        std::cout << "After repair, AVL? " << (tree.isAVL() ? "yes" : "no") << "\n";
    } else {
        std::cout << "No repair needed.\n";
    }

    return 0;
}
