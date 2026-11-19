#ifndef AVL_REPAIR_INT_HPP
#define AVL_REPAIR_INT_HPP

#include <optional>
#include <string>
#include <vector>

class AVLNode {
public:
    int key;
    int height;
    AVLNode* left_ptr;
    AVLNode* right_ptr;

    explicit AVLNode(int k)
        : key(k), height(1), left_ptr(nullptr), right_ptr(nullptr) {
    }
};

enum class RotationCase {
    None = 0,
    LL,
    LR,
    RR,
    RL,
    L0,
    R0
};

struct RepairStep {
    bool foundBadNode;
    int badKey;
    int badBF;
    int childKey;
    int childBF;
    RotationCase rotationCase;
    int newSubtreeRootKey;
};

class AVLRepairInt {
public:
    AVLRepairInt();
    ~AVLRepairInt();

    AVLRepairInt(const AVLRepairInt&) = delete;
    AVLRepairInt& operator=(const AVLRepairInt&) = delete;

    void clear();
    void loadFromLevelOrder(const std::vector<std::optional<int>>& values);

    int recomputeHeights();
    bool isAVL() const;

    RepairStep analyzeNextRepair();
    bool repairNextImbalance(RepairStep& outStep);
    std::vector<RepairStep> repairUntilAVL();

    std::vector<int> inorderKeys() const;
    std::vector<std::optional<int>> levelOrderWithNulls() const;
    const AVLNode* root() const;

private:
    AVLNode* root_ptr;

    void deleteRec(AVLNode* node);
    AVLNode* buildFromLevelOrderRec(const std::vector<std::optional<int>>& values, int index);
    void inorderRec(const AVLNode* node, std::vector<int>& out) const;

    int heightOf(const AVLNode* node) const;
    int recomputeHeightsRec(AVLNode* node);
    int balanceFactor(const AVLNode* node) const;
    void updateHeight(AVLNode* node);

    bool isAVLRec(const AVLNode* node) const;
    int actualHeightRec(const AVLNode* node, bool& ok) const;

    AVLNode* rotateLeft(AVLNode* node);
    AVLNode* rotateRight(AVLNode* node);

    AVLNode* findDeepestImbalance(AVLNode* node);
    RotationCase classifyCase(const AVLNode* bad) const;
    int predictedNewRootKey(const AVLNode* bad, RotationCase kind) const;

    AVLNode* repairAtKeyRec(AVLNode* node, int badKey, bool& repaired);
};

std::string rotationCaseToString(RotationCase rotationCase);
bool sameShapeAndKeys(const AVLNode* a, const AVLNode* b);

#endif
