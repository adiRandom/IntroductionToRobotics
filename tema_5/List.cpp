#ifndef LIST_CPP
#define LIST_CPP

template<class T>
class List {

public:
  struct Node {
    T value;
    Node* next = nullptr;
  };

private:
  Node* _head = nullptr;
  Node* _tail = nullptr;

  void cleanup() {
    Node* node = _head;

    while (node != nullptr) {
      Node* current = node;
      node = node->next;
      delete current;
    }

    _head = nullptr;
    _tail = nullptr;
  }

  void copy(List const& other) {
    if (&other == this) {
      return;
    }

    cleanup();

    if (other._head == nullptr) {
      return;
    }

    _head = new Node;
    _head->value = other._head->value;

    Node* node = _head;
    Node* otherNode = other._head->next;
    while (otherNode != nullptr) {
      Node* newNode = new Node;
      newNode->value = otherNode->value;
      node->next = newNode;
      node = newNode;
      otherNode = otherNode->next;
    }

    _tail = node;
  }

public:
  List() {
  }

  List<T>(List<T> const& other) {
    copy(other);
  }

  List& operator=(List other) {
    copy(other);
    return *this;
  }

  Node* getHead() {
    return _head;
  }

  void push(T value) {
    if (_head == nullptr) {
      _head = new Node;
      _head->value = value;
      _tail = _head;
      return;
    }

    Node* node = new Node;
    node->value = value;
    _tail->next = node;
    _tail = node;
  }

  void forEach(void (*func)(T)) {
    Node* node = _head;

    while (node != nullptr) {
      func(node->value);
      node = node->next;
    }
  }

  ~List() {
    cleanup();
  }
};

#endif