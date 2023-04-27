class Wheel {
public:
    Wheel() : current_index(0) {
    }

    void rotate_left(int steps) {
        current_index = (current_index + steps) % bufferSize;
    }

    void rotate_right(int steps) {
        current_index = (current_index - steps + bufferSize) % bufferSize;
    }

    void rotate_to(char target) {
        int target_pos = find(target);
        if (target_pos == -1) {
            Serial.println("Target character not found in wheel.");
            return;
        }

        int left_steps = (target_pos - current_index + bufferSize) % bufferSize;
        int right_steps = (current_index - target_pos + bufferSize) % bufferSize;

        if (left_steps <= right_steps) {
            rotate_left(left_steps);
        } else {
            rotate_right(right_steps);
        }
    }

    char get_top() const {
        return buffer[current_index];
    }

private:
    static const size_t bufferSize = 26;
    char buffer[bufferSize] = {'_','a', 'b', 'c', 'X', 'y', 'm', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', 'x', '/', '(', ')', '^', '='};
    size_t current_index;

    int find(char target) {
        for (int i = 0; i < bufferSize; ++i) {
            if (buffer[i] == target) {
                return i;
            }
        }
        return -1;
    }

    int distance(int index1, int index2) {
        return index2 - index1;
    }
};
