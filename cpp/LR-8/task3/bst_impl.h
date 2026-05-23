template<typename K, typename V>
BST<K,V>::iterator::iterator(Node* ptr, Node* root) : current(ptr), treeRoot(root) {}

template<typename K, typename V>
std::pair<const K,V>& BST<K,V>::iterator::operator*() const {
    return current->data;
}

template<typename K, typename V>
typename BST<K,V>::iterator& BST<K,V>::iterator::operator++() {
    current = successor(current);
    return *this;
}

template<typename K, typename V>
typename BST<K,V>::iterator BST<K,V>::iterator::operator++(int) {
    iterator tmp = *this;
    ++(*this);
    return tmp;
}

template<typename K, typename V>
bool BST<K,V>::iterator::operator==(const iterator& other) const { return current == other.current; }
template<typename K, typename V>
bool BST<K,V>::iterator::operator!=(const iterator& other) const { return !(*this == other); }

template<typename K, typename V>
typename BST<K,V>::Node* BST<K,V>::iterator::successor(Node* node) const {
    if (!node) return nullptr;
    if (node->right) {
        Node* cur = node->right.get();
        while (cur->left) cur = cur->left.get();
        return cur;
    }
    Node* par = node->parent;
    while (par && node == par->right.get()) {
        node = par;
        par = par->parent;
    }
    return par;
}

template<typename K, typename V>
typename BST<K,V>::iterator BST<K,V>::tree_begin() {
    Node* cur = root.get();
    if (!cur) return iterator(nullptr, nullptr);
    while (cur->left) cur = cur->left.get();
    return iterator(cur, root.get());
}
template<typename K, typename V>
typename BST<K,V>::iterator BST<K,V>::tree_end() { return iterator(nullptr, root.get()); }

template<typename K, typename V>
BST<K,V>::list_iterator::list_iterator(Node* ptr) : current(ptr) {}
template<typename K, typename V>
std::pair<const K,V>& BST<K,V>::list_iterator::operator*() const { return current->data; }
template<typename K, typename V>
typename BST<K,V>::list_iterator& BST<K,V>::list_iterator::operator++() {
    current = current->next;
    return *this;
}
template<typename K, typename V>
typename BST<K,V>::list_iterator BST<K,V>::list_iterator::operator++(int) {
    list_iterator tmp = *this;
    ++(*this);
    return tmp;
}
template<typename K, typename V>
bool BST<K,V>::list_iterator::operator==(const list_iterator& other) const { return current == other.current; }
template<typename K, typename V>
bool BST<K,V>::list_iterator::operator!=(const list_iterator& other) const { return !(*this == other); }

template<typename K, typename V>
typename BST<K,V>::list_iterator BST<K,V>::list_begin() { return list_iterator(listHead); }
template<typename K, typename V>
typename BST<K,V>::list_iterator BST<K,V>::list_end() { return list_iterator(nullptr); }

template<typename K, typename V>
bool BST<K,V>::insert(const K& key, const V& value) {
    Node* ins = insertNode(root, nullptr, key, value);
    if (ins) {
        ++sz;
        return true;
    }
    return false;
}

template<typename K, typename V>
typename BST<K,V>::Node* BST<K,V>::insertNode(std::unique_ptr<Node>& node, Node* parent, const K& key, const V& value) {
    if (!node) {
        node = std::make_unique<Node>(key, value);
        node->parent = parent;
        return node.get();
    }
    if (key < node->data.first)
        return insertNode(node->left, node.get(), key, value);
    else if (key > node->data.first)
        return insertNode(node->right, node.get(), key, value);
    else return nullptr;
}

template<typename K, typename V>
bool BST<K,V>::erase(const K& key) {
    bool deleted = false;
    root = std::unique_ptr<Node>(eraseNode(std::move(root), key, deleted));
    if (deleted) --sz;
    return deleted;
}

template<typename K, typename V>
typename BST<K,V>::Node* BST<K,V>::eraseNode(std::unique_ptr<Node>& node, const K& key, bool& deleted) {
    if (!node) return nullptr;
    if (key < node->data.first) {
        node->left = std::unique_ptr<Node>(eraseNode(std::move(node->left), key, deleted));
        if (node->left) node->left->parent = node.get();
    } else if (key > node->data.first) {
        node->right = std::unique_ptr<Node>(eraseNode(std::move(node->right), key, deleted));
        if (node->right) node->right->parent = node.get();
    } else {
        deleted = true;
        Node* ret = nullptr;
        if (!node->left) {
            ret = node->right.release();
            if (ret) ret->parent = node->parent;
        } else if (!node->right) {
            ret = node->left.release();
            if (ret) ret->parent = node->parent;
        } else {
            Node* min = findMin(node->right.get());
            node->data.~pair();
            new (&node->data) std::pair<const K,V>(min->data);
            node->right = std::unique_ptr<Node>(eraseNode(std::move(node->right), min->data.first, deleted));
            if (node->right) node->right->parent = node.get();
            return node.release();
        }
        return ret;
    }
    return node.release();
}

template<typename K, typename V>
typename BST<K,V>::Node* BST<K,V>::findNode(Node* node, const K& key) const {
    while (node) {
        if (key < node->data.first) node = node->left.get();
        else if (key > node->data.first) node = node->right.get();
        else return node;
    }
    return nullptr;
}

template<typename K, typename V>
typename BST<K,V>::Node* BST<K,V>::find(const K& key) const {
    return findNode(root.get(), key);
}

template<typename K, typename V>
typename BST<K,V>::Node* BST<K,V>::findMin(Node* node) const {
    while (node && node->left) node = node->left.get();
    return node;
}

template<typename K, typename V>
void BST<K,V>::buildList() {
    listHead = listTail = nullptr;
    Node* prev = nullptr;
    std::function<void(Node*)> link = [&](Node* node) {
        if (!node) return;
        link(node->left.get());

        node->prev = prev;
        if (prev) prev->next = node;
        else listHead = node;
        prev = node;
        link(node->right.get());
    };
    link(root.get());
    listTail = prev;
    if (listTail) listTail->next = nullptr;
}

template<typename K, typename V>
void BST<K,V>::inorder(std::function<void(const K&, const V&)> visit) const {
    inorder(root.get(), visit);
}

template<typename K, typename V>
void BST<K,V>::inorder(Node* node, std::function<void(const K&, const V&)> visit) const {
    if (!node) return;
    inorder(node->left.get(), visit);
    visit(node->data.first, node->data.second);
    inorder(node->right.get(), visit);
}