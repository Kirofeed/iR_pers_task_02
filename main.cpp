#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>
#include <set>

using namespace std;

struct Node {
    Node* parent;
    Node* left;
    Node* right;
    int height;
    int msl;
    int value;
    Node(int val, Node* parent = nullptr) : parent(parent), left(nullptr), right(nullptr), height(0), msl(0), value(val) {}
};

class Tree {
private:
    Node* root;
    int max_msl = INT_MIN;

public:
    Tree() : root(nullptr) {}
    void insert_node(int val);
    void marking_heights_msl(Node* node);
    void collect_max_msl_nodes(Node* node, int max_msl, vector<Node*>& max_nodes);
    Node* find_deepest_node(Node* node);
    void collect_all_path_nodes(Node* node, set<Node*>& path_nodes);
    vector<Node*> sorted_nodes(const set<Node*>& path_nodes);
    void delete_node(Node* node);
    Node* get_root() const { return root; }
    int Get_max_msl();
    Node* Get_root();
    void get_max_semipath_nodes(Node* node, set<Node*>& path_nodes);
    void printing(Node* nodie, ostream& os);
};

void Tree::insert_node(int val) {
    Node** parent = nullptr;
    Node** cur = &this->root;
    while (*cur != nullptr) {
        if (val > (*cur)->value) {
            parent = cur;
            cur = &(*cur)->right;
        }
        else if (val < (*cur)->value) {
            parent = cur;
            cur = &(*cur)->left;
        }
        else {
            return;
        }
    }
    if (parent == nullptr) {
        *cur = new Node(val, nullptr);
    } else {
        *cur = new Node(val, *parent);
    }
}

void Tree::marking_heights_msl(Node* node) {
    if (node) {
        marking_heights_msl(node->left);
        marking_heights_msl(node->right);
        if (node->left && node->right) {
            node->height = max(node->left->height, node->right->height) + 1;
            node->msl = node->right->height + node->left->height + 2;
        }
        else if (!node->left && node->right) {
            node->height = node->right->height + 1;
            node->msl = node->right->height + 1;
        }
        else if (!node->right && node->left) {
            node->height = node->left->height + 1;
            node->msl = node->left->height + 1;
        }
        if (node->msl > max_msl) {
            max_msl = node->msl;
        }
    }
}

void Tree::collect_max_msl_nodes(Node* node, int max_msl, vector<Node*>& max_nodes) {
    if (!node) return;
    if (node->msl == max_msl) {
        max_nodes.push_back(node);
    }
    collect_max_msl_nodes(node->left, max_msl, max_nodes);
    collect_max_msl_nodes(node->right, max_msl, max_nodes);
}

void Tree::get_max_semipath_nodes(Node* node, set<Node*>& path_nodes) {
    Node** current = &node;
    path_nodes.emplace(node);
    while ((*current)->left || (*current)->right) {
        if (!(*current)->right) {
            current = &(*current)->left;
            path_nodes.emplace(*current);
        }
        else if (!(*current)->left) {
            current = &(*current)->right;
            path_nodes.emplace(*current);
        }
        else {
            if ((*current)->left->height > (*current)->right->height) {
                current = &(*current)->left;
                path_nodes.emplace(*current);
            }
            else if ((*current)->left->height < (*current)->right->height) {
                current = &(*current)->right;
                path_nodes.emplace(*current);
            }
            else {
                get_max_semipath_nodes((*current)->left, path_nodes);
                get_max_semipath_nodes((*current)->right, path_nodes);
                return;
            }
        }
    }
    return;
    
}

void Tree::collect_all_path_nodes(Node* node, set<Node*>& path_nodes) {
    path_nodes.emplace(node);
    if (node->left) {
        get_max_semipath_nodes(node->left, path_nodes);
    }
    if (node->right) {
        get_max_semipath_nodes(node->right, path_nodes);
    }
    return;
}

vector<Node*> Tree::sorted_nodes(const set<Node*>& path_nodes) {
    vector<Node*> sorted;
    for (auto node : path_nodes) {
        sorted.push_back(node);
    }
    sort(sorted.begin(), sorted.end(), [](Node* a, Node* b) {
        return a->value < b->value;
    });
    return sorted;
}

void Tree::delete_node(Node* node) {
    if (node == nullptr) return;

    if (node->left == nullptr && node->right == nullptr) {
        if (node->parent == nullptr) {
            root = nullptr;
        } else {
            if (node->parent->left == node) {
                node->parent->left = nullptr;
            } else {
                node->parent->right = nullptr;
            }
        }
        delete node;
    }
    else if (node->left == nullptr) {
        if (node->parent == nullptr) {
            root = node->right;
            root->parent = nullptr;
        } else {
            if (node->parent->left == node) {
                node->parent->left = node->right;
            } else {
                node->parent->right = node->right;
            }
            node->right->parent = node->parent;
        }
        delete node;
    }
    else if (node->right == nullptr) {
        if (node->parent == nullptr) {
            root = node->left;
            root->parent = nullptr;
        } else {
            if (node->parent->left == node) {
                node->parent->left = node->left;
            } else {
                node->parent->right = node->left;
            }
            node->left->parent = node->parent;
        }
        delete node;
    }
    else {
        Node* successor = node->right;
        while (successor->left != nullptr) {
            successor = successor->left;
        }

        node->value = successor->value;

        delete_node(successor);
    }
}


int Tree::Get_max_msl() {
    return this->max_msl;
}

Node* Tree::Get_root() {
    return this->root;
}

void Tree::printing(Node* nodie, ostream& os) {
    if (nodie == nullptr) {
        return;
    }
    os << nodie->value << endl;
    printing(nodie->left, os);
    printing(nodie->right, os);
    return;
}

int main () {
    Tree tree;
    ifstream in("in.txt");
    if (!in.is_open())
    {
        cout << "problem with opening";
        return 1;
    }
    
    int input;
    while (in >> input) {
        tree.insert_node(input);
    }
    vector<int> values;

    tree.marking_heights_msl(tree.get_root());

    int max_msl = tree.Get_max_msl();

    vector<Node*> max_nodes;
    tree.collect_max_msl_nodes(tree.get_root(), max_msl, max_nodes);

    set<Node*> all_path_nodes;
    for (auto node : max_nodes) {
        tree.collect_all_path_nodes(node, all_path_nodes);
    }

    vector<Node*> sorted = tree.sorted_nodes(all_path_nodes);
    for (auto i : sorted) {
        cout << i->value << "\t";
    }

    if (sorted.size() >= 2) {
        Node* second_node = sorted[1];
        tree.delete_node(second_node);
    }


    ofstream out("out.txt");
    tree.printing(tree.get_root(), out);
    return 0;
}
