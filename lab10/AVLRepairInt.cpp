#include "AVLRepairInt.hpp"

#include <queue>

AVLRepairInt::AVLRepairInt()
    : root_ptr(nullptr) {
}

AVLRepairInt::~AVLRepairInt() {
    clear();
}

void AVLRepairInt::clear() {
    deleteRec(root_ptr);
    root_ptr = nullptr;
}

void AVLRepairInt::deleteRec(AVLNode* node) {
    if (node == nullptr) {
        return;
    }

    deleteRec(node->left_ptr);
    deleteRec(node->right_ptr);
    delete node;
}

AVLNode* AVLRepairInt::buildFromLevelOrderRec(
    const std::vector<std::optional<int>>& values,
    int index) {
    if (index < 0 || index >= static_cast<int>(values.size())) {
        return nullptr;
    }

    if (!values[index].has_value()) {
        return nullptr;
    }

    AVLNode* node = new AVLNode(*values[index]);
    node->left_ptr = buildFromLevelOrderRec(values, index * 2 + 1);
    node->right_ptr = buildFromLevelOrderRec(values, index * 2 + 2);

    return node;
}

void AVLRepairInt::loadFromLevelOrder(
    const std::vector<std::optional<int>>& values) {
    clear();
    root_ptr = buildFromLevelOrderRec(values, 0);
}

void AVLRepairInt::inorderRec(
    const AVLNode* node,
    std::vector<int>& out) const {
    if (node == nullptr) {
        return;
    }

    inorderRec(node->left_ptr, out);
    out.push_back(node->key);
    inorderRec(node->right_ptr, out);
}

std::vector<int> AVLRepairInt::inorderKeys() const {
    std::vector<int> out;
    inorderRec(root_ptr, out);
    return out;
}

std::vector<std::optional<int>> AVLRepairInt::levelOrderWithNulls() const {
    std::vector<std::optional<int>> out;

    if (root_ptr == nullptr) {
        return out;
    }

    std::queue<const AVLNode*> q;
    q.push(root_ptr);

    while (!q.empty()) {
        const AVLNode* current = q.front();
        q.pop();

        if (current == nullptr) {
            out.push_back(std::nullopt);
        } else {
            out.push_back(current->key);
            q.push(current->left_ptr);
            q.push(current->right_ptr);
        }
    }

    while (!out.empty() && !out.back().has_value()) {
        out.pop_back();
    }

    return out;
}

const AVLNode* AVLRepairInt::root() const {
    return root_ptr;
}

int AVLRepairInt::heightOf(const AVLNode* node) const {
    return node == nullptr ? 0 : node->height;
}

int AVLRepairInt::recomputeHeightsRec(AVLNode* node) {
    if (node == nullptr) {
        return 0;
    }

    const int leftHeight = recomputeHeightsRec(node->left_ptr);
    const int rightHeight = recomputeHeightsRec(node->right_ptr);

    node->height = 1 + (leftHeight > rightHeight
        ? leftHeight
        : rightHeight);

    return node->height;
}

int AVLRepairInt::recomputeHeights() {
    return recomputeHeightsRec(root_ptr);
}

int AVLRepairInt::balanceFactor(const AVLNode* node) const {
    if (node == nullptr) {
        return 0;
    }

    return heightOf(node->left_ptr) - heightOf(node->right_ptr);
}

void AVLRepairInt::updateHeight(AVLNode* node) {
    if (node == nullptr) {
        return;
    }

    const int leftHeight = heightOf(node->left_ptr);
    const int rightHeight = heightOf(node->right_ptr);

    node->height = 1 + (leftHeight > rightHeight
        ? leftHeight
        : rightHeight);
}

int AVLRepairInt::actualHeightRec(
    const AVLNode* node,
    bool& ok) const {
    if (node == nullptr) {
        return 0;
    }

    const int leftHeight = actualHeightRec(node->left_ptr, ok);
    const int rightHeight = actualHeightRec(node->right_ptr, ok);

    if (leftHeight - rightHeight > 1 ||
        rightHeight - leftHeight > 1) {
        ok = false;
    }

    return 1 + (leftHeight > rightHeight
        ? leftHeight
        : rightHeight);
}

bool AVLRepairInt::isAVLRec(const AVLNode* node) const {
    bool ok = true;
    actualHeightRec(node, ok);
    return ok;
}

bool AVLRepairInt::isAVL() const {
    return isAVLRec(root_ptr);
}

AVLNode* AVLRepairInt::rotateLeft(AVLNode* node) {
    if (node == nullptr || node->right_ptr == nullptr) {
        return node;
    }

    AVLNode* newRoot = node->right_ptr;

    node->right_ptr = newRoot->left_ptr;
    newRoot->left_ptr = node;

    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

AVLNode* AVLRepairInt::rotateRight(AVLNode* node) {
    if (node == nullptr || node->left_ptr == nullptr) {
        return node;
    }

    AVLNode* newRoot = node->left_ptr;

    node->left_ptr = newRoot->right_ptr;
    newRoot->right_ptr = node;

    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

AVLNode* AVLRepairInt::findDeepestImbalance(AVLNode* node) {
    if (node == nullptr) {
        return nullptr;
    }

    AVLNode* leftResult =
        findDeepestImbalance(node->left_ptr);

    if (leftResult != nullptr) {
        return leftResult;
    }

    AVLNode* rightResult =
        findDeepestImbalance(node->right_ptr);

    if (rightResult != nullptr) {
        return rightResult;
    }

    const int bf = balanceFactor(node);

    if (bf > 1 || bf < -1) {
        return node;
    }

    return nullptr;
}

RotationCase AVLRepairInt::classifyCase(
    const AVLNode* bad) const {
    if (bad == nullptr) {
        return RotationCase::None;
    }

    const int badBF = balanceFactor(bad);

    if (badBF > 1) {
        const int childBF = balanceFactor(bad->left_ptr);

        if (childBF > 0) {
            return RotationCase::LL;
        }

        if (childBF == 0) {
            return RotationCase::L0;
        }

        return RotationCase::LR;
    }

    if (badBF < -1) {
        const int childBF = balanceFactor(bad->right_ptr);

        if (childBF < 0) {
            return RotationCase::RR;
        }

        if (childBF == 0) {
            return RotationCase::R0;
        }

        return RotationCase::RL;
    }

    return RotationCase::None;
}

int AVLRepairInt::predictedNewRootKey(
    const AVLNode* bad,
    RotationCase kind) const {
    if (bad == nullptr || kind == RotationCase::None) {
        return 0;
    }

    if (kind == RotationCase::LL ||
        kind == RotationCase::L0) {
        return bad->left_ptr != nullptr
            ? bad->left_ptr->key
            : 0;
    }

    if (kind == RotationCase::LR) {
        return bad->left_ptr != nullptr &&
               bad->left_ptr->right_ptr != nullptr
            ? bad->left_ptr->right_ptr->key
            : 0;
    }

    if (kind == RotationCase::RR ||
        kind == RotationCase::R0) {
        return bad->right_ptr != nullptr
            ? bad->right_ptr->key
            : 0;
    }

    return bad->right_ptr != nullptr &&
           bad->right_ptr->left_ptr != nullptr
        ? bad->right_ptr->left_ptr->key
        : 0;
}

RepairStep AVLRepairInt::analyzeNextRepair() {
    RepairStep step{};

    step.foundBadNode = false;
    step.badKey = 0;
    step.badBF = 0;
    step.childKey = 0;
    step.childBF = 0;
    step.rotationCase = RotationCase::None;
    step.newSubtreeRootKey = 0;

    recomputeHeights();

    AVLNode* bad = findDeepestImbalance(root_ptr);

    if (bad == nullptr) {
        return step;
    }

    step.foundBadNode = true;
    step.badKey = bad->key;
    step.badBF = balanceFactor(bad);

    AVLNode* child = step.badBF > 1
        ? bad->left_ptr
        : bad->right_ptr;

    if (child != nullptr) {
        step.childKey = child->key;
        step.childBF = balanceFactor(child);
    }

    step.rotationCase = classifyCase(bad);
    step.newSubtreeRootKey =
        predictedNewRootKey(bad, step.rotationCase);

    return step;
}

AVLNode* AVLRepairInt::repairAtKeyRec(
    AVLNode* node,
    int badKey,
    bool& repaired) {
    if (node == nullptr) {
        return nullptr;
    }

    node->left_ptr =
        repairAtKeyRec(node->left_ptr, badKey, repaired);

    if (repaired) {
        return node;
    }

    node->right_ptr =
        repairAtKeyRec(node->right_ptr, badKey, repaired);

    if (repaired) {
        return node;
    }

    if (node->key != badKey) {
        return node;
    }

    const RotationCase kind = classifyCase(node);
    repaired = true;

    if (kind == RotationCase::LL ||
        kind == RotationCase::L0) {
        return rotateRight(node);
    }

    if (kind == RotationCase::RR ||
        kind == RotationCase::R0) {
        return rotateLeft(node);
    }

    if (kind == RotationCase::LR) {
        node->left_ptr = rotateLeft(node->left_ptr);
        return rotateRight(node);
    }

    if (kind == RotationCase::RL) {
        node->right_ptr = rotateRight(node->right_ptr);
        return rotateLeft(node);
    }

    repaired = false;
    return node;
}

bool AVLRepairInt::repairNextImbalance(
    RepairStep& outStep) {
    outStep = analyzeNextRepair();

    if (!outStep.foundBadNode) {
        return false;
    }

    bool repaired = false;

    root_ptr =
        repairAtKeyRec(root_ptr, outStep.badKey, repaired);

    recomputeHeights();

    return repaired;
}

std::vector<RepairStep> AVLRepairInt::repairUntilAVL() {
    std::vector<RepairStep> steps;
    RepairStep step{};

    while (repairNextImbalance(step)) {
        steps.push_back(step);
    }

    return steps;
}

std::string rotationCaseToString(
    RotationCase rotationCase) {
    switch (rotationCase) {
        case RotationCase::LL: return "LL";
        case RotationCase::LR: return "LR";
        case RotationCase::RR: return "RR";
        case RotationCase::RL: return "RL";
        case RotationCase::L0: return "L0";
        case RotationCase::R0: return "R0";
        default: return "None";
    }
}

bool sameShapeAndKeys(
    const AVLNode* a,
    const AVLNode* b) {
    if (a == nullptr && b == nullptr) {
        return true;
    }

    if (a == nullptr || b == nullptr) {
        return false;
    }

    if (a->key != b->key) {
        return false;
    }

    return sameShapeAndKeys(a->left_ptr, b->left_ptr) &&
           sameShapeAndKeys(a->right_ptr, b->right_ptr);
}
