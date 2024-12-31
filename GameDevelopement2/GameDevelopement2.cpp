#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>
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

// Merge Sort Step
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

// Quick Sort Step
struct QuickState {
    std::stack<std::pair<int, int>> st; // Pair for low and high indices
    int low;
    int high;
    int i;
    int j;
    int pivotIndex;
    int partitionStep;
    int step;
    bool partitionComplete;
};

void partition(std::vector<int>& array, QuickState& quickState) {
    int low = quickState.low;
    int high = quickState.high;
    int& i = quickState.i;
    int& j = quickState.j;

    if (quickState.partitionStep == 1) {
        i = low - 1;
        j = low;
        quickState.partitionStep = 2; // Start processing
    }

    if (quickState.partitionStep == 2) {
        int pivot = array[high];

        if (j <= high - 1) {
            if (array[j] < pivot) {
                std::swap(array[++i], array[j]);
            }
            j++;
        }
        else {
            std::swap(array[i + 1], array[high]);
            quickState.pivotIndex = ++i;
            quickState.partitionStep = 1; // Reset for the next partition
            quickState.partitionComplete = true;
        }
    }
}

void quickSortStep(std::vector<int>& array, QuickState& quickState, bool& sorted) {
    if (quickState.step == 1) {
        // Initialize stack with the first range
        quickState.st.push({ quickState.low, quickState.high });
        quickState.step = 2;
    }

    if (quickState.step == 2) {
        if (!quickState.st.empty()) {
            int low = quickState.st.top().first;
            int high = quickState.st.top().second;
            quickState.st.pop();
            quickState.low = low;
            quickState.high = high;
            quickState.partitionComplete = false;
            quickState.step = 3;
        }
        else {
            sorted = true;
        }
    }

    if (quickState.step == 3) {
        if (!quickState.partitionComplete) {
            partition(array, quickState);
        }
        else {
            quickState.step = 4; // Move to the next partitioning phase
        }
    }

    if (quickState.step == 4) {
        int pivot = quickState.pivotIndex;

        // Push subranges to the stack for further sorting
        if (pivot - 1 > quickState.low) {
            quickState.st.push({ quickState.low, pivot - 1 });
        }
        if (pivot + 1 < quickState.high) {
            quickState.st.push({ pivot + 1, quickState.high });
        }

        quickState.step = 2; // Go back to process the next range
    }
}


// Heap Sort Step
struct HeapifyState {
    int i;            // Current node index
    int n;            // Size of the heap
    int largest;      // Index of the largest value
    int left;         // Index of the left child
    int right;        // Index of the right child
    int step;         // Current step in heapify
    bool complete;    // Indicates if heapify is done
};

// Function to initialize heapify state
void initializeHeapifyState(HeapifyState& heapifyState, int i, int n) {
    heapifyState.i = i;
    heapifyState.n = n;
    heapifyState.largest = i;
    heapifyState.left = 2 * i + 1;
    heapifyState.right = 2 * i + 2;
    heapifyState.step = 1;
    heapifyState.complete = false;
}

// Step-by-step heapify function
void heapifyStep(std::vector<int>& array, HeapifyState& heapifyState) {
    int i = heapifyState.i;
    int n = heapifyState.n;

    if (heapifyState.step == 1) {
        // Compare root with the left child
        if (heapifyState.left < n && array[heapifyState.left] > array[heapifyState.largest]) {
            heapifyState.largest = heapifyState.left;
        }
        heapifyState.step++;
    }
    else if (heapifyState.step == 2) {
        // Compare root with the right child
        if (heapifyState.right < n && array[heapifyState.right] > array[heapifyState.largest]) {
            heapifyState.largest = heapifyState.right;
        }
        heapifyState.step++;
    }
    else if (heapifyState.step == 3) {
        // Swap if necessary and recurse
        if (heapifyState.largest != i) {
            std::swap(array[i], array[heapifyState.largest]);
            initializeHeapifyState(heapifyState, heapifyState.largest, n); // Reinitialize for the subtree
        }
        else {
            heapifyState.complete = true; // No swaps needed, heapify is done
        }
    }
}

// Heap Sort Step-by-Step with Heapify Visualization
struct HeapState {
    int heapSize;            // Current size of the heap
    int buildStep;           // Tracks heap building steps
    int sortStep;            // Tracks heap sorting steps
    bool buildComplete;      // Flag indicating if heap is built
    HeapifyState heapifyState; // State of current heapify operation
};

void heapSortStep(std::vector<int>& array, HeapState& heapState, bool& sorted) {
    if (!heapState.buildComplete) {
        // Build the max heap step by step
        if (heapState.heapifyState.complete) {
            if (heapState.buildStep < heapState.heapSize / 2) {
                int i = (heapState.heapSize / 2 - 1) - heapState.buildStep;
                initializeHeapifyState(heapState.heapifyState, i, heapState.heapSize);
                heapState.buildStep++;
            }
            else {
                heapState.buildComplete = true;
            }
        }
        else {
            heapifyStep(array, heapState.heapifyState);
        }
    }
    else {
        // Sort the heap step by step
        if (heapState.sortStep < heapState.heapSize - 1) {
            if (heapState.heapifyState.complete) {
                int i = heapState.heapSize - 1 - heapState.sortStep;
                std::swap(array[0], array[i]); // Move max to the end
                initializeHeapifyState(heapState.heapifyState, 0, i); // Reinitialize heapify for remaining heap
                heapState.sortStep++;
            }
            else {
                heapifyStep(array, heapState.heapifyState);
            }
        }
        else {
            sorted = true; // Sorting is complete
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
    std::vector<int> bubbleArray(NUM_BARS), insertionArray(NUM_BARS), selectionArray(NUM_BARS), mergeArray(NUM_BARS), quickArray(NUM_BARS), heapArray(NUM_BARS);
    for (int i = 0; i < NUM_BARS; i++) {
        int height = (std::rand() % (WINDOW_HEIGHT/2 - 50)) + 10; // we add 10 minimum height, so that the value is not 0 which will make it invisible to see the bar
        bubbleArray[i] = height;
        insertionArray[i] = height;
        selectionArray[i] = height;
        mergeArray[i] = height;
        quickArray[i] = height;
        heapArray[i] = height;
    }

    // Variables for sorting
    int bubbleI = 0, bubbleJ = 0, insertionI = 1, insertionJ = 1, selectionI = 0, selectionJ = 0, selectionMinIndex = 0, mergeSize = 1, mergeLeft = 0;
    bool bubbleSorted = false, insertionSorted = false, selectionSorted = false, mergeSorted = false, mergingFlag = false, quickSorted = false, heapSorted = false;
    bool consolePrintBubbleSortTime = false, consolePrintInsertionSortTime = false, consolePrintSelectionSortTime = false, consolePrintMergeSortTime = false, consolePrintQuickSortTime = false,consolePrintHeapSortTime = false, consolePrintTotalTime = false;

    // Timers for sorting algorithms
    sf::Clock bubbleClock, insertionClock, selectionClock, mergeClock, quickClock, heapClock, totalClock;
    float bubbleElapsed = 0, insertionElapsed = 0, selectionElapsed = 0, mergeElapsed = 0, quickElapsed = 0, heapElapsed = 0, totalElapsed = 0;

    MergeState mergeState;
    QuickState quickState;
    quickState.low = 0;
    quickState.high = quickArray.size()-1;
    quickState.step = 1;
    quickState.partitionStep = 1;
    quickState.partitionComplete = false;

    HeapState heapState;
    heapState.heapSize = static_cast<int> (heapArray.size());
    heapState.buildStep = 0;
    heapState.sortStep = 0;
    heapState.buildComplete = false;

    int heapi = heapState.heapSize / 2 - 1;
    heapState.heapifyState = { heapi, heapState.heapSize, heapi, 2*heapi + 1, 2*heapi - 1, 1, false };

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
        if (!mergeSorted) {
            mergeClock.restart();
            mergeSortStep(mergeArray, mergeSize, mergeLeft, mergeSorted, mergeState, mergingFlag);
            mergeElapsed += mergeClock.getElapsedTime().asSeconds();
        }

        // Perform Quick Sort step and update timer
        if (!quickSorted) {
            quickClock.restart();
            quickSortStep(quickArray, quickState, quickSorted);
            quickElapsed += quickClock.getElapsedTime().asSeconds();
        }

        // Perform  Heap Sort step and update timer
        if (!heapSorted) {
            heapClock.restart();
            heapSortStep(heapArray, heapState, heapSorted);
            heapElapsed += heapClock.getElapsedTime().asSeconds();
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

        // Draw Quick Sort bars
        if (!quickSorted) {
            quickClock.restart();
            drawBars(window, quickArray, WINDOW_WIDTH / 3, WINDOW_HEIGHT, BAR_WIDTH, sf::Color::White);
            quickElapsed += quickClock.getElapsedTime().asSeconds();
        }
        else {
            if (!consolePrintQuickSortTime) {
                consolePrintQuickSortTime = true;
                std::cout << "Quick Sorting Completed in " << std::to_string(quickElapsed) << " seconds!\n";
            }
            drawBars(window, quickArray, WINDOW_WIDTH / 3, WINDOW_HEIGHT, BAR_WIDTH, sf::Color::White);
        }

        // Draw Heap Sort bars
        if (!heapSorted) {
            heapClock.restart();
            drawBars(window, heapArray, WINDOW_WIDTH * 2 / 3, WINDOW_HEIGHT, BAR_WIDTH, sf::Color::Magenta);
            heapElapsed += heapClock.getElapsedTime().asSeconds();
        }
        else {
            if (!consolePrintHeapSortTime) {
                consolePrintHeapSortTime = true;
                std::cout << "Heap Sorting Completed in " << std::to_string(heapElapsed) << " seconds!\n";
            }
            drawBars(window, heapArray, WINDOW_WIDTH * 2 / 3, WINDOW_HEIGHT, BAR_WIDTH, sf::Color::Magenta);
        }

        // Display Total Sorting time for all the sort algorithms to complete
        if (!consolePrintTotalTime && consolePrintBubbleSortTime && consolePrintInsertionSortTime && consolePrintSelectionSortTime && consolePrintMergeSortTime && consolePrintQuickSortTime && consolePrintHeapSortTime) {
            std::cout << "All Sorting Completed in " << std::to_string(totalClock.getElapsedTime().asSeconds()) << " seconds!\n";
            consolePrintTotalTime = true;
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

        sf::Text quickTimeText("Quick Sort Time: " + formatTime(quickElapsed), font, 20);
        quickTimeText.setFillColor(sf::Color::White);
        quickTimeText.setPosition(WINDOW_WIDTH / 3 + 10, WINDOW_HEIGHT / 2 + 10);
        window.draw(quickTimeText);

        sf::Text heapTimeText("Heap Sort Time: " + formatTime(heapElapsed), font, 20);
        heapTimeText.setFillColor(sf::Color::White);
        heapTimeText.setPosition(WINDOW_WIDTH * 2/ 3 + 10, WINDOW_HEIGHT / 2 + 10);
        window.draw(heapTimeText);

        window.display();

        // Slow down the visualization for clarity
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}
