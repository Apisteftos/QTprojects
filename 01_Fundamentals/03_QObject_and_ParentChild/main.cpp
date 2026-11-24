// 03_QObject_and_ParentChild: ownership in Qt. A parent deletes its children.

#include <QDebug>
#include <QObject>
#include <QPointer>

// A QObject that reports its birth and death.
class Node : public QObject
{
public:
    explicit Node(const QString &name, QObject *parent = nullptr)
        : QObject(parent)
    {
        setObjectName(name);
        say("  created   " + objectName());
    }

    ~Node() override { say("  destroyed " + objectName()); }

    static void say(const QString &text) { qDebug().noquote() << text; }
};

static void title(const QString &text)
{
    qDebug().noquote() << "";
    qDebug().noquote() << "=== " + text + " ===";
}

int main()
{
    // === EXAMPLE 1: a parent deletes its children ===
    title("Example 1: deleting the root deletes the whole tree");
    {
        auto *root = new Node("root");
        new Node("child A", root);
        auto *childB = new Node("child B", root);
        new Node("grandchild", childB);

        Node::say("  delete root");
        delete root;
    }

    // === EXAMPLE 2: looking at the tree ===
    title("Example 2: parent(), children(), findChild()");
    {
        Node root("root");
        auto *a = new Node("child A", &root);
        auto *b = new Node("child B", &root);

        Node::say(QString("  root has %1 children").arg(root.children().size()));
        Node::say("  parent of child A: " + a->parent()->objectName());
        Node::say("  findChild(\"child B\") found: " + root.findChild<QObject *>("child B")->objectName());
        Node::say(b->parent() == &root ? "  child B belongs to root" : "  ?");
    }

    // === EXAMPLE 3: deleting a child removes it from its parent ===
    title("Example 3: a deleted child leaves the parent's list");
    {
        Node root("root");
        auto *a = new Node("child A", &root);
        new Node("child B", &root);

        delete a;
        Node::say(QString("  root now has %1 child").arg(root.children().size()));
    }

    // === EXAMPLE 4: giving a child to another parent ===
    title("Example 4: setParent() moves a child");
    {
        Node first("first");
        Node second("second");
        auto *child = new Node("child", &first);

        Node::say(QString("  first: %1, second: %2").arg(first.children().size()).arg(second.children().size()));
        child->setParent(&second);
        Node::say(QString("  first: %1, second: %2").arg(first.children().size()).arg(second.children().size()));
    }

    // === EXAMPLE 5: QPointer knows when the object is gone ===
    title("Example 5: QPointer becomes null");
    {
        auto *root = new Node("root");
        auto *child = new Node("child", root);
        QPointer<Node> guard(child);

        Node::say(guard ? "  guard is valid" : "  guard is null");
        delete root; // deletes child too
        Node::say(guard ? "  guard is valid" : "  guard is null");
    }

    return 0;
}
