#include <huffman.h>
#include <stack>

class HuffmanTree::Impl {
public:
    struct Node {
        Node* l = nullptr;
        Node* r = nullptr;
        Node* pred = nullptr;
        size_t deep = 0;
        uint8_t a = 255;
    };
    Node* root = nullptr;
    Node* now = nullptr;
    Node* MakeNodePtr(uint8_t v = 255) {
        Node* n = new Node();
        n->a = v;
        return n;
    }

    void DeleteTree(Node* node) {
        if (!node) {
            return;
        }

        std::stack<Node*> nodes_to_delete;
        nodes_to_delete.push(node);

        while (!nodes_to_delete.empty()) {
            Node* current = nodes_to_delete.top();
            nodes_to_delete.pop();

            if (current->l) {
                nodes_to_delete.push(current->l);
            }
            if (current->r) {
                nodes_to_delete.push(current->r);
            }
            delete current;
        }

        root = nullptr;
    }

    bool PutInd(Node* node, size_t ind, uint8_t v) {
        if (node->a != 255) {
            return false;
        }
        if (ind == 0) {
            node->a = v;
            now = node;
            return true;
        }
        if (!node->l) {
            node->l = MakeNodePtr();
            node->l->pred = node;
            node->l->deep = node->deep + 1;
            if (ind == 1) {
                node->l->a = v;
                now = node->l;
                return true;
            }
        }
        if (PutInd(node->l, ind - 1, v)) {
            return true;
        }
        if (!node->r) {
            node->r = MakeNodePtr();
            node->r->pred = node;
            node->r->deep = node->deep + 1;
            if (ind == 1) {
                node->r->a = v;
                now = node->r;
                return true;
            }
        }
        if (PutInd(node->r, ind - 1, v)) {
            return true;
        }
        return false;
    }
};

HuffmanTree::HuffmanTree() {
    impl_ = std::make_unique<Impl>();
}

void HuffmanTree::Build(const std::vector<uint8_t>& code_lengths,
                        const std::vector<uint8_t>& values) {
    if (impl_->root) {
        impl_->DeleteTree(impl_->root);
    }
    impl_->root = impl_->MakeNodePtr();
    impl_->now = impl_->root;
    size_t ind = 0;
    for (size_t i = 0; i < code_lengths.size(); ++i) {
        for (int j = code_lengths[i]; j > 0; --j) {
            if (ind >= values.size()) {
                impl_->DeleteTree(impl_->root);
                throw std::invalid_argument("");
            }
            if (!impl_->PutInd(impl_->root, i + 1, values[ind++])) {
                impl_->now = nullptr;
                impl_->DeleteTree(impl_->root);
                throw std::invalid_argument("");
            }
        }
        if (impl_->now->deep > 16) {
            impl_->now = nullptr;
            impl_->DeleteTree(impl_->root);
            throw std::invalid_argument("tree too big");
        }
    }
    impl_->now = impl_->root;
}

bool HuffmanTree::Move(bool bit, int& value) {
    if (!impl_->now) {
        throw std::invalid_argument("");
    }
    if (!bit) {
        if (!impl_->now->l) {
            throw std::invalid_argument("");
        }
        impl_->now = impl_->now->l;
        if (impl_->now->a != 255) {
            value = impl_->now->a;
            impl_->now = impl_->root;
            return true;
        }
        return false;
    }
    if (!impl_->now->r) {
        throw std::invalid_argument("");
    }
    impl_->now = impl_->now->r;
    if (impl_->now->a != 255) {
        value = impl_->now->a;
        impl_->now = impl_->root;
        return true;
    }
    return false;
}

HuffmanTree::HuffmanTree(HuffmanTree&&) = default;

HuffmanTree& HuffmanTree::operator=(HuffmanTree&&) = default;

HuffmanTree::~HuffmanTree() {
    impl_->DeleteTree(impl_->root);
}