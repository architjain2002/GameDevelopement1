#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <iomanip>
#include <sstream>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Sorting bars
const int NUM_BARS = 100;
const float BAR_WIDTH = static_cast<float>(WINDOW_WIDTH) / (3 * NUM_BARS);



void drawBars(sf::RenderWindow& window, const std::vector<int>& array, int startX, int startY, float barWidth, sf::Color color);
// Bubble Sort Step
void bubbleSortStep(std::vector<int>& array, int& i, int& j, bool& sorted) {
    if (i < array.size() - 1) {
        if (j < array.size() - i - 1) {
            if (array[j] > array[j + 1]) {
                std::swap(array[j], array[j + 1]);
            }
            j++;
        }
        else {
            j = 0;
            i++;
        }
    }
    else {
        sorted = true;
    }
}

// Insertion Sort Step
void insertionSortStep(std::vector<int>& array, int& i, int& j, bool& sorted) {
    if (i < array.size()) {
        if (j > 0 && array[j - 1] > array[j]) {
            std::swap(array[j], array[j - 1]);
            j--;
        }
        else {
            i++;
            j = i;
        }
    }
    else {
        sorted = true;
    }
}

// Selection Sort Step
void selectionSortStep(std::vector<int>& array, int& i, int& j, int& minIndex, bool& sorted) {
    if (i < array.size()-1) {
        // int minIndex = i;
        if (j < array.size()) {
            if (array[j] < array[minIndex]) {
                minIndex = j;
            }
            j++;
        }
        else {
            std:: swap(array[i], array[minIndex]);
            i++;
            minIndex = i;
            j = i + 1;
        }
    }
    else {
        sorted = true;
    }
}

//void merge(std::vector<int>& array, int left, int mid, int right, std::vector<int> temp, sf::RenderWindow& window) {
//    int i = left, j = mid + 1, k = left;
//
//
//    // Merge the two subarrays into a temporary array
//    while (i <= mid && j <= right) {
//        if (array[i] <= array[j]) {
//            temp[k++] = array[i++];
//        }
//        else {
//            temp[k++] = array[j++];
//        }
//    }
//
//    // Copy remaining elements from left subarray
//    while (i <= mid) {
//        temp[k++] = array[i++];
//    }
//
//    // Copy remaining elements from right subarray
//    while (j <= right) {
//        temp[k++] = array[j++];
//    }
//
//    // Copy the sorted elements back into the original array
//    for (int idx = left; idx <= right; idx++) {
//        array[idx] = temp[idx];
//    }
//}
//
//
//// Merge Sort Step
//void mergeSortStep(std::vector<int>& array, std::vector<int> temp, int& size, int& left, bool& sorted, sf::RenderWindow& window) {
//    int n = array.size();
//
//    if (size >= n) {
//        sorted = true; // If the size exceeds the array, sorting is complete
//        return;
//    }
//
//    int mid = std::min(left + size - 1, n-1);      // Calculate mid-
//
//    int right = std::min(left + 2*size - 1, n - 1); // Ensure the right index doesn't exceed array bounds
//
//    // Merge the current segment
//    merge(array, left, mid, right, temp, window);
//
//    // Move to the next segment
//    left += 2*size;
//
//    // If the entire array has been processed, double the size of segments
//    if (left >= n) {
//        left = 0;  // Reset left index for the next pass
//        size *= 2; // Double the size of the segments
//    }
//}

struct MergeState {
    int left;           // Left index of the current segment
    int mid;            // Midpoint of the current segment
    int right;          // Right index of the current segment
    int i;              // Index for the left subarray
    int j;              // Index for the right subarray
    int k;              // Index for the merged array
    int n1;             // Size of the left subarray
    int n2;             // Size of the right subarray
    std::vector<int> L; // Left subarray
    std::vector<int> R; // Right subarray
    bool mergeComplete; // Flag to indicate merge completion
};

void initializeMergeState(std::vector<int>& array, MergeState& state, int left, int mid, int right) {
    state.left = left;
    state.mid = mid;
    state.right = right;
    state.n1 = mid - left + 1;
    state.n2 = right - mid;
    state.i = 0;
    state.j = 0;
    state.k = left;
    state.L.assign(array.begin() + left, array.begin() + mid + 1);
    state.R.assign(array.begin() + mid + 1, array.begin() + right + 1);
    state.mergeComplete = false;
}

void mergeStep(std::vector<int>& array, MergeState& state) {
    if (state.i < state.n1 && state.j < state.n2) {
        if (state.L[state.i] <= state.R[state.j]) {
            array[state.k] = state.L[state.i];
            state.i++;
        }
        else {
            array[state.k] = state.R[state.j];
            state.j++;
        }
        state.k++;
    }
    else if (state.i < state.n1) {
        array[state.k] = state.L[state.i];
        state.i++;
        state.k++;
    }
    else if (state.j < state.n2) {
        array[state.k] = state.R[state.j];
        state.j++;
        state.k++;
    }
    else {
        state.mergeComplete = true; // Merge for this step is done
    }
}


void mergeSortStep(std::vector<int>& array, int& size, int& left, bool& sorted, MergeState& mergeState, bool& merging) {
    int n = array.size();

    if (size >= n) {
        sorted = true; // Sorting is complete
        return;
    }

    // Initialize merge state if not currently merging
    if (!merging) {
        int mid = std::min(left + size - 1, n - 1);           // Calculate midpoint
        int right = std::min(left + 2 * size - 1, n - 1);     // Calculate right index
        initializeMergeState(array, mergeState, left, mid, right);
        merging = true;
    }
    else {
        // Perform a single step of merging
        mergeStep(array, mergeState);

        // If merging is complete, proceed to the next segment
        if (mergeState.mergeComplete) {
            left += 2 * size;
            merging = false; // Reset merging state

            // If all segments are processed, double the size
            if (left >= n) {
                left = 0;
                size *= 2;
                if (size >= n) {
                    sorted = true;
                }
            }
        }
    }
}



// Draw bars for sorting visualization
void drawBars(sf::RenderWindow& window, const std::vector<int>& array, int startX, int startY, float barWidth, sf::Color color) {
    for (size_t i = 0; i < array.size(); i++) {
        sf::RectangleShape bar(sf::Vector2f(barWidth - 1, array[i]));
        bar.setPosition(startX + i * barWidth, startY - array[i]);
        bar.setFillColor(color);
        window.draw(bar);
    }
}

// Format time as a string
std::string formatTime(float timeInSeconds) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << timeInSeconds << " s";
    return oss.str();
}

int main() {
    // Initialize window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Sorting Visualization");

    // Load font for timer display
    sf::Font font;
    if (!font.loadFromFile("C:\\archit_3\\open-sans\\OpenSans-Regular.ttf")) {
        std::cerr << "Error: Could not load font.\n";
        return -1;
    }

    // Randomize the height of bars
    std::srand(static_cast<unsigned>(std::time(0)));
    std::vector<int> bubbleArray(NUM_BARS), insertionArray(NUM_BARS), selectionArray(NUM_BARS), mergeArray(NUM_BARS), tempMergeArray(NUM_BARS);
    for (int i = 0; i < NUM_BARS; i++) {
        int height = (std::rand() % (WINDOW_HEIGHT/2 - 50)) + 10; // we add 10 minimum height, so that the value is not 0 which will make it invisible to see the bar
        bubbleArray[i] = height;
        insertionArray[i] = height;
        selectionArray[i] = height;
        mergeArray[i] = height;
    }

    // Variables for sorting
    int bubbleI = 0, bubbleJ = 0, insertionI = 1, insertionJ = 1, selectionI = 0, selectionJ = 0, selectionMinIndex = 0, mergeSize = 1, mergeLeft = 0;
    bool bubbleSorted = false, insertionSorted = false, selectionSorted = false, mergeSorted = false, mergingFlag = false;
    bool consolePrintBubbleSortTime = false, consolePrintInsertionSortTime = false, consolePrintSelectionSortTime = false, consolePrintMergeSortTime = false;

    // Timers for sorting algorithms
    sf::Clock bubbleClock, insertionClock, selectionClock, mergeClock;
    float bubbleElapsed = 0, insertionElapsed = 0, selectionElapsed = 0, mergeElapsed = 0;

    MergeState mergeState;
    

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Render visualization
        window.clear(sf::Color::Black);

        // Perform Bubble Sort step and update timer
        if (!bubbleSorted) {
            bubbleClock.restart();  // Start timing Bubble Sort step
            bubbleSortStep(bubbleArray, bubbleI, bubbleJ, bubbleSorted);
            bubbleElapsed += bubbleClock.getElapsedTime().asSeconds();
        }

        // Perform Insertion Sort step and update timer
        if (!insertionSorted) {
            insertionClock.restart();  // Start timing Insertion Sort step
            insertionSortStep(insertionArray, insertionI, insertionJ, insertionSorted);
            insertionElapsed += insertionClock.getElapsedTime().asSeconds();
        }

        // Perform Selection Sort step and update timer
        if (!selectionSorted) {
            selectionClock .restart();  // Start timing Insertion Sort step
            selectionSortStep(selectionArray, selectionI, selectionJ,selectionMinIndex, selectionSorted);
            selectionElapsed += selectionClock.getElapsedTime().asSeconds();
        }

        // Perform Merge Sort step and update timer
        //if (!mergeSorted) {
        //    mergeClock.restart();  // Start timing Insertion Sort step
        //    mergeSortStep(mergeArray, mergeArray, mergeSize, mergeLeft, mergeSorted, window);
        //    mergeElapsed += mergeClock.getElapsedTime().asSeconds();
        //}

        if (!mergeSorted) {
            mergeClock.restart();
            mergeSortStep(mergeArray, mergeSize, mergeLeft, mergeSorted, mergeState, mergingFlag);
            mergeElapsed += mergeClock.getElapsedTime().asSeconds();
        }

        // Draw Bubble Sort bars
        if (!bubbleSorted) {
            bubbleClock.restart();
            drawBars(window, bubbleArray, 0, WINDOW_HEIGHT / 2, BAR_WIDTH, sf::Color::Red);
            bubbleElapsed += bubbleClock.getElapsedTime().asSeconds();
        }
        else {
            if (!consolePrintBubbleSortTime) {
                consolePrintBubbleSortTime = true;
                std::cout << "Bubble Sorting Completed in " << std::to_string(bubbleElapsed) << " seconds!\n";
            }
            drawBars(window, bubbleArray, 0,WINDOW_HEIGHT/2, BAR_WIDTH, sf::Color::Red);
        }

        // Draw Insertion Sort bars
        if (!insertionSorted) {
            insertionClock.restart();
            drawBars(window, insertionArray, WINDOW_WIDTH / 3, WINDOW_HEIGHT / 2, BAR_WIDTH, sf::Color::Blue);
            insertionElapsed += insertionClock.getElapsedTime().asSeconds();
        }
        else {
            if (!consolePrintInsertionSortTime) {
                consolePrintInsertionSortTime = true;
                std::cout << "Insertion Sorting Completed in " << std::to_string(insertionElapsed) << " seconds!\n";
            }
            drawBars(window, insertionArray, WINDOW_WIDTH / 3, WINDOW_HEIGHT / 2, BAR_WIDTH, sf::Color::Blue);
        }

        // Draw Selection Sort bars
        if (!selectionSorted) {
            selectionClock.restart();
            drawBars(window, selectionArray, WINDOW_WIDTH* 2 / 3, WINDOW_HEIGHT / 2, BAR_WIDTH, sf::Color::Green);
            selectionElapsed += selectionClock.getElapsedTime().asSeconds();
        }
        else {
            if (!consolePrintSelectionSortTime) {
                consolePrintSelectionSortTime = true;
                std::cout << "Selection Sorting Completed in " << std::to_string(selectionElapsed) << " seconds!\n";            
            }
            drawBars(window, selectionArray, WINDOW_WIDTH * 2 / 3, WINDOW_HEIGHT / 2, BAR_WIDTH, sf::Color::Green);
        }

        // Draw Merge Sort bars
        if(!mergeSorted){
            mergeClock.restart();
            drawBars(window, mergeArray, 0, WINDOW_HEIGHT, BAR_WIDTH, sf::Color::Yellow);
            mergeElapsed += mergeClock.getElapsedTime().asSeconds();
        }
        else {
            if (!consolePrintMergeSortTime) {
                consolePrintMergeSortTime = true;
                std::cout << "Merge Sorting Completed in " << std::to_string(mergeElapsed) << " seconds!\n";
            }
            drawBars(window, mergeArray, 0, WINDOW_HEIGHT, BAR_WIDTH, sf::Color::Yellow);
        }

        // Display sorting times
        sf::Text bubbleTimeText("Bubble Sort Time: " + formatTime(bubbleElapsed), font, 20);
        bubbleTimeText.setFillColor(sf::Color::White);
        bubbleTimeText.setPosition(10, 10);
        window.draw(bubbleTimeText);

        sf::Text insertionTimeText("Insertion Sort Time: " + formatTime(insertionElapsed), font, 20);
        insertionTimeText.setFillColor(sf::Color::White);
        insertionTimeText.setPosition(WINDOW_WIDTH/ 3 + 10, 10);
        window.draw(insertionTimeText);

        sf::Text selectionTimeText("Selection Sort Time: " + formatTime(selectionElapsed), font, 20);
        selectionTimeText.setFillColor(sf::Color::White);
        selectionTimeText.setPosition(WINDOW_WIDTH * 2 / 3 + 10, 10);
        window.draw(selectionTimeText);

        sf::Text mergeTimeText("Merge Sort Time: " + formatTime(mergeElapsed), font, 20);
        mergeTimeText.setFillColor(sf::Color::White);
        mergeTimeText.setPosition(10, WINDOW_HEIGHT/2 + 10);
        window.draw(mergeTimeText);

        window.display();

        // Slow down the visualization for clarity
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
