#include <iostream>
#include <vector>
#include <string>
#include <stack>


class TextEditor;
class Action {
public:
    virtual void redo(TextEditor&) = 0;
    virtual void undo(TextEditor&) = 0;
};

class TextEditor {
private:
    std::string text;
    size_t pos;
    std::stack<Action*> undo_stack;
    std::stack<Action*> redo_stack;
public:
    TextEditor(): pos(0) {}
    TextEditor(const std::string& data) {
        pos = 0;
        for (auto i: data) {
            EventHandler(i);
        }
    }

    void EventHandler(const char& c);

    void Print() {
        std::cout << text;
    }

    friend class InsertChar;
    friend class ChangePos;
};

class InsertChar: public Action {
private:
    char symbol;
    size_t position;
public:
    InsertChar(char c, size_t pos): symbol(c), position(pos) {}
    void redo(TextEditor& txt) {
        txt.text.insert(txt.text.begin() + position, symbol);
        ++txt.pos;
    }
    void undo(TextEditor& txt) {
        if (position != txt.text.length() - 1) {
            std::string new_text = txt.text.substr(0, position) + txt.text.substr(position + 1, txt.text.length() - position);
            txt.text = new_text;
        } else {
            std::string new_text = txt.text.substr(0, position);
            txt.text = new_text;
        }
        --txt.pos;
    }
};

class ChangePos: public Action {
private:
    int direct;
public:
    ChangePos(int n): direct(n) {}
    void redo(TextEditor& txt) { txt.pos += direct; }
    void undo(TextEditor& txt) { txt.pos -= direct; }
};

void TextEditor::EventHandler(const char& c) {
    if (c == '<') {
        redo_stack = std::stack<Action*>();
        if (pos > 0) {
            --pos;
            ChangePos* t = new ChangePos(-1);
            undo_stack.push(t);
        }
    } else if (c == '>') {
        redo_stack = std::stack<Action*>();
        ++pos;
        if (pos == text.size() + 1) {
            text.insert(text.begin() + pos - 1, ' ');
        }
        ChangePos* t = new ChangePos(1);
        undo_stack.push(t);
    } else if (c == '#') {
        if (!undo_stack.empty()) {
            redo_stack.push(undo_stack.top());
            undo_stack.top()->undo(*this);
            undo_stack.pop();
        }
    } else if (c == '*') {
        if (!redo_stack.empty()) {
            redo_stack.top()->redo(*this);
            undo_stack.push(redo_stack.top());
            redo_stack.pop();
        }
    } else {
        redo_stack = std::stack<Action*>();
        InsertChar* cp = new InsertChar(c, pos);
        undo_stack.push(cp);
        text.insert(text.begin() + pos, c);
        ++pos;
    }
}


int main() {
    std::string data;
    std::cin >> data;
    TextEditor MyPad(data);
    MyPad.Print();
    return 0;
}