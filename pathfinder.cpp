#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <chrono>
#include <thread>
#include <iostream>

const int GRID_SIZE = 20;
const int WINDOW_SIZE = 800;
const int CELL_SIZE = WINDOW_SIZE / GRID_SIZE;
const int DELAY_MS = 50; // Delay in milliseconds after each iteration

class PathfindingVisualizer {
public:
    PathfindingVisualizer() : window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Pathfinding Visualizer") {
        initializeGrid();
        run();
    }

private:
    sf::RenderWindow window;
    std::vector<std::vector<sf::RectangleShape>> grid;
    sf::Vector2i startCell;
    sf::Vector2i endCell;
    bool pathFound;
    std::vector<std::vector<sf::Vector2i>> parent;

    void initializeGrid() {
        grid.resize(GRID_SIZE, std::vector<sf::RectangleShape>(GRID_SIZE));

        // Initialize the grid
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                grid[i][j].setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                grid[i][j].setPosition(j * CELL_SIZE, i * CELL_SIZE);
                grid[i][j].setFillColor(sf::Color(75,54,95));
                // Set border properties
                grid[i][j].setOutlineThickness(1); // Border thickness
                grid[i][j].setOutlineColor(sf::Color::Black); // Border color
            }
        }
    }

    void findShortestPath() {
        // Perform Dijkstra's algorithm
        std::vector<std::vector<bool>> visited(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
        parent.resize(GRID_SIZE, std::vector<sf::Vector2i>(GRID_SIZE));
        std::vector<std::vector<int>> distance(GRID_SIZE, std::vector<int>(GRID_SIZE, std::numeric_limits<int>::max()));
        std::queue<sf::Vector2i> q;
        q.push(startCell);
        distance[startCell.x][startCell.y] = 0;

        while (!q.empty()) {
            sf::Vector2i current = q.front();
            q.pop();
            visited[current.x][current.y] = true;

            if (current == endCell) {
                // Reached the end cell, stop the algorithm
                break;
            }

            // Define the deltas for moving in four directions: up, down, left, and right
            std::vector<sf::Vector2i> deltas = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

            // Iterate over the horizontally and vertically neighboring cells
            for (const auto& delta : deltas) {
                sf::Vector2i neighbor(current.x + delta.x, current.y + delta.y);

                // Check if the neighboring cell is within the grid bounds and not a wall
                if (neighbor.x >= 0 && neighbor.x < GRID_SIZE && neighbor.y >= 0 && neighbor.y < GRID_SIZE &&
                    !visited[neighbor.x][neighbor.y]) {
                    int newDistance = distance[current.x][current.y] + 1;

                    if (newDistance < distance[neighbor.x][neighbor.y]) {
                        distance[neighbor.x][neighbor.y] = newDistance;
                        parent[neighbor.x][neighbor.y] = current;
                        grid[neighbor.x][neighbor.y].setFillColor(sf::Color(223,215,200)); // Color the visited node as black
                        window.clear();
                        drawGrid(); // Draw the updated grid
                        window.display();
                        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS)); // Delay for visualization
                        q.push(neighbor);
                    }
                }
            }
        }
        pathFound=true;
    }

    void findAndColorShortestPath() {
        // Trace back the path from the end cell to the start cell
        sf::Vector2i current = endCell;
        while (current != startCell) {
            grid[current.x][current.y].setFillColor(sf::Color::Yellow);
            current = parent[current.x][current.y];
        }
    }

    void drawGrid() {
        // Draw the grid
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                window.draw(grid[i][j]);
            }
        }
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (!pathFound && event.type == sf::Event::MouseButtonPressed) {
                    handleMouseClick(event.mouseButton);
                }

                if (!pathFound && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    findShortestPath();
                    findAndColorShortestPath();
                }
            }

            window.clear();
            drawGrid();
            window.display();
        }
    }
    void handleMouseClick(sf::Event::MouseButtonEvent mouseEvent) {
        int row = mouseEvent.y / CELL_SIZE;
        int col = mouseEvent.x / CELL_SIZE;

        if (mouseEvent.button == sf::Mouse::Left) {
            // Left mouse click sets the start cell
            startCell = sf::Vector2i(row, col);
            grid[row][col].setFillColor(sf::Color(170,219,30));
        } else if (mouseEvent.button == sf::Mouse::Right) {
            // Right mouse click sets the end cell
            endCell = sf::Vector2i(row, col);
            grid[row][col].setFillColor(sf::Color(244,54,76));
        }
    }
};

class PathfindingVisualizer1 {
public:
    PathfindingVisualizer1() : window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Pathfinding Visualizer") {
        initializeGrid();
        run();
    }

private:
    sf::RenderWindow window;
    std::vector<std::vector<sf::RectangleShape>> grid;
    sf::Vector2i startCell;
    sf::Vector2i endCell;
    bool pathFound;
    std::vector<std::vector<sf::Vector2i>> parent;

    void initializeGrid() {
        grid.resize(GRID_SIZE, std::vector<sf::RectangleShape>(GRID_SIZE));

        // Initialize the grid
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                grid[i][j].setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                grid[i][j].setPosition(j * CELL_SIZE, i * CELL_SIZE);
                grid[i][j].setFillColor(sf::Color(75,54,95));
                grid[i][j].setOutlineThickness(1); // Border thickness
                grid[i][j].setOutlineColor(sf::Color::Black); // Border color
            }
        }
    }

    void findShortestPath() {
        // Perform Depth-First Search
        std::vector<std::vector<bool>> visited(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
        parent.resize(GRID_SIZE, std::vector<sf::Vector2i>(GRID_SIZE));
        std::stack<sf::Vector2i> stk;
        stk.push(startCell);

        while (!stk.empty()) {
            sf::Vector2i current = stk.top();
            stk.pop();

            if (current == endCell) {
                // Reached the end cell, stop the algorithm
                break;
            }

            visited[current.x][current.y] = true;

            // Define the deltas for moving in four directions: up, down, left, and right
            std::vector<sf::Vector2i> deltas = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

            // Iterate over the horizontally and vertically neighboring cells
            for (const auto& delta : deltas) {
                sf::Vector2i neighbor(current.x + delta.x, current.y + delta.y);

                // Check if the neighboring cell is within the grid bounds and not a wall
                if (neighbor.x >= 0 && neighbor.x < GRID_SIZE && neighbor.y >= 0 && neighbor.y < GRID_SIZE &&
                    !visited[neighbor.x][neighbor.y]) {
                    parent[neighbor.x][neighbor.y] = current;
                    grid[neighbor.x][neighbor.y].setFillColor(sf::Color(223,215,200)); // Color the visited node as black
                    window.clear();
                    drawGrid(); // Draw the updated grid
                    window.display();
                    std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS)); // Delay for visualization
                    stk.push(neighbor);
                }
            }
        }
    pathFound=true;

    }

    void findAndColorShortestPath() {
        // Trace back the path from the end cell to the start cell
        sf::Vector2i current = endCell;
        while (current != startCell) {
            grid[current.x][current.y].setFillColor(sf::Color::Yellow);
            current = parent[current.x][current.y];
        }
    }

    void drawGrid() {
        // Draw the grid
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                window.draw(grid[i][j]);
            }
        }
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (!pathFound && event.type == sf::Event::MouseButtonPressed) {
                    handleMouseClick(event.mouseButton);
                }

                if (!pathFound && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    findShortestPath();
                    findAndColorShortestPath();
                }
            }

            window.clear();
            drawGrid();
            window.display();
        }
    }
    void handleMouseClick(sf::Event::MouseButtonEvent mouseEvent) {
        int row = mouseEvent.y / CELL_SIZE;
        int col = mouseEvent.x / CELL_SIZE;

        if (mouseEvent.button == sf::Mouse::Left) {
            // Left mouse click sets the start cell
            startCell = sf::Vector2i(row, col);
            grid[row][col].setFillColor(sf::Color(170,219,30));
        } else if (mouseEvent.button == sf::Mouse::Right) {
            // Right mouse click sets the end cell
            endCell = sf::Vector2i(row, col);
            grid[row][col].setFillColor(sf::Color(244,54,76));
        }
    }
};

struct Cell {
    int heuristic;
    sf::Vector2i coordinates;

    // Define custom comparison operator for the priority queue
    bool operator<(const Cell& other) const {
        return heuristic > other.heuristic; // Note: Using > for min-heap behavior
    }
};

class PathfindingVisualizer2 {
public:
    PathfindingVisualizer2() : window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Pathfinding Visualizer") {
        initializeGrid();
        run();
    }

private:
    sf::RenderWindow window;
    std::vector<std::vector<sf::RectangleShape>> grid;
    sf::Vector2i startCell;
    sf::Vector2i endCell;
    bool pathFound;
    std::vector<std::vector<sf::Vector2i>> parent;

    void initializeGrid() {
        grid.resize(GRID_SIZE, std::vector<sf::RectangleShape>(GRID_SIZE));

        // Initialize the grid
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                grid[i][j].setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                grid[i][j].setPosition(j * CELL_SIZE, i * CELL_SIZE);
                grid[i][j].setFillColor(sf::Color(75,54,95));
                grid[i][j].setOutlineThickness(1); // Border thickness
                grid[i][j].setOutlineColor(sf::Color::Black); // Border color
            }
        }
    }

    // Heuristic function for Greedy Best-first Search
    int heuristic(const sf::Vector2i& current) {
        return abs(current.x - endCell.x) + abs(current.y - endCell.y);
    }

    void findShortestPath() {
        // Perform Greedy Best-first Search
        std::vector<std::vector<bool>> visited(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
        parent.resize(GRID_SIZE, std::vector<sf::Vector2i>(GRID_SIZE));
        std::priority_queue<Cell> pq;
        pq.push({heuristic(startCell), startCell});

        while (!pq.empty()) {
            Cell current = pq.top();
            pq.pop();
            visited[current.coordinates.x][current.coordinates.y] = true;

            if (current.coordinates == endCell) {
                // Reached the end cell, stop the algorithm
                break;
            }

            // Define the deltas for moving in four directions: up, down, left, and right
            std::vector<sf::Vector2i> deltas = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

            // Explore neighbors
            for (const auto& delta : deltas) {
                sf::Vector2i neighbor(current.coordinates.x + delta.x, current.coordinates.y + delta.y);

                // Check if the neighboring cell is within the grid bounds and not a wall
                if (neighbor.x >= 0 && neighbor.x < GRID_SIZE && neighbor.y >= 0 && neighbor.y < GRID_SIZE &&
                    !visited[neighbor.x][neighbor.y]) {
                    parent[neighbor.x][neighbor.y] = current.coordinates;
                    grid[neighbor.x][neighbor.y].setFillColor(sf::Color(223,215,200)); // Color the visited node as black
                    window.clear();
                    drawGrid(); // Draw the updated grid
                    window.display();
                    std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS)); // Delay for visualization
                    pq.push({heuristic(neighbor), neighbor});
                }
            }
        }
        pathFound=true;
    }

    void findAndColorShortestPath() {
        // Trace back the path from the end cell to the start cell
        sf::Vector2i current = endCell;
        while (current != startCell) {
            grid[current.x][current.y].setFillColor(sf::Color::Yellow);
            current = parent[current.x][current.y];
        }
    }

    void drawGrid() {
        // Draw the grid
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                window.draw(grid[i][j]);
            }
        }
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (!pathFound && event.type == sf::Event::MouseButtonPressed) {
                    handleMouseClick(event.mouseButton);
                }

                if (!pathFound && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    findShortestPath();
                    findAndColorShortestPath();
                }
            }

            window.clear();
            drawGrid();
            window.display();
        }
    }
    void handleMouseClick(sf::Event::MouseButtonEvent mouseEvent) {
        int row = mouseEvent.y / CELL_SIZE;
        int col = mouseEvent.x / CELL_SIZE;

        if (mouseEvent.button == sf::Mouse::Left) {
            // Left mouse click sets the start cell
            startCell = sf::Vector2i(row, col);
            grid[row][col].setFillColor(sf::Color(170,219,30));
        } else if (mouseEvent.button == sf::Mouse::Right) {
            // Right mouse click sets the end cell
            endCell = sf::Vector2i(row, col);
            grid[row][col].setFillColor(sf::Color(244,54,76));
        }
    }
};

struct Compare {
    bool operator()(const std::pair<int, sf::Vector2i>& a, const std::pair<int, sf::Vector2i>& b) const {
        // Compare based on the first element of the pairs
        return a.first > b.first;
    }
};


class PathfindingVisualizer3 {
public:
    PathfindingVisualizer3() : window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Pathfinding Visualizer") {
        initializeGrid();
        run();
    }

private:
    sf::RenderWindow window;
    std::vector<std::vector<sf::RectangleShape>> grid;
    sf::Vector2i startCell;
    sf::Vector2i endCell;
    bool pathFound;
    std::vector<std::vector<sf::Vector2i>> parent;

    void initializeGrid() {
        grid.resize(GRID_SIZE, std::vector<sf::RectangleShape>(GRID_SIZE));

        // Initialize the grid
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                grid[i][j].setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                grid[i][j].setPosition(j * CELL_SIZE, i * CELL_SIZE);
                grid[i][j].setFillColor(sf::Color(75,54,95));
                grid[i][j].setOutlineThickness(1); // Border thickness
                grid[i][j].setOutlineColor(sf::Color::Black); // Border color
            }
        }
    }

    // Heuristic function for A* algorithm
    int heuristic(const sf::Vector2i& current) {
        return abs(current.x - endCell.x) + abs(current.y - endCell.y);
    }

    void findShortestPath() {
    // Perform A* algorithm
    
    parent.resize(GRID_SIZE, std::vector<sf::Vector2i>(GRID_SIZE, sf::Vector2i(-1, -1)));
    std::priority_queue<std::pair<int, sf::Vector2i>, std::vector<std::pair<int, sf::Vector2i>>, Compare> pq;
    pq.push({heuristic(startCell), startCell});
     std::vector<std::vector<bool>> visited(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));
     std::vector<std::vector<int>> distance(GRID_SIZE, std::vector<int>(GRID_SIZE, std::numeric_limits<int>::max()));
        distance[startCell.x][startCell.y] = 0;

    while (!pq.empty()) {
        sf::Vector2i current = pq.top().second;
        pq.pop();

        if (current == endCell) {
            // Reached the end cell, stop the algorithm
            break;
        }

        // Define the deltas for moving in four directions: up, down, left, and right
        std::vector<sf::Vector2i> deltas = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

        // Iterate over the horizontally and vertically neighboring cells
        for (const auto& delta : deltas) {
            sf::Vector2i neighbor(current.x + delta.x, current.y + delta.y);

            // Check if the neighboring cell is within the grid bounds and not a wall
            if (neighbor.x >= 0 && neighbor.x < GRID_SIZE && neighbor.y >= 0 && neighbor.y < GRID_SIZE) {
                int newDistance = distance[current.x][current.y] + 1;

                if (newDistance < distance[neighbor.x][neighbor.y]) {
                    distance[neighbor.x][neighbor.y] = newDistance;
                    parent[neighbor.x][neighbor.y] = current;
                    grid[neighbor.x][neighbor.y].setFillColor(sf::Color(223,215,200)); // Color the visited node as blue
                    pq.push({newDistance + heuristic(neighbor), neighbor});
                    window.clear();
                    drawGrid(); // Draw the updated grid
                    window.display();
                    std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS)); // Delay for visualization
                }
            }
        }
    }
    pathFound = true;
}


    void findAndColorShortestPath() {
        // Trace back the path from the end cell to the start cell
        sf::Vector2i current = endCell;
        while (current != startCell) {
            grid[current.x][current.y].setFillColor(sf::Color::Yellow);
            current = parent[current.x][current.y];
        }
    }

    void drawGrid() {
        // Draw the grid
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                window.draw(grid[i][j]);
            }
        }
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (!pathFound && event.type == sf::Event::MouseButtonPressed) {
                    handleMouseClick(event.mouseButton);
                }

                if (!pathFound && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    findShortestPath();
                    findAndColorShortestPath();
                }
            }

            window.clear();
            drawGrid();
            window.display();
        }
    }
    void handleMouseClick(sf::Event::MouseButtonEvent mouseEvent) {
        int row = mouseEvent.y / CELL_SIZE;
        int col = mouseEvent.x / CELL_SIZE;

        if (mouseEvent.button == sf::Mouse::Left) {
            // Left mouse click sets the start cell
            startCell = sf::Vector2i(row, col);
            grid[row][col].setFillColor(sf::Color(170,219,30));
        } else if (mouseEvent.button == sf::Mouse::Right) {
            // Right mouse click sets the end cell
            endCell = sf::Vector2i(row, col);
            grid[row][col].setFillColor(sf::Color(244,54,76));
        }
    }
};
class MainVisualizerPage {
public:
    MainVisualizerPage() : window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Main Page") {
        createButtons();
        run();
    }

private:
    sf::RenderWindow window;
    std::vector<sf::RectangleShape> buttons;

    void createButtons() {
        sf::Vector2f buttonSize(200, 50);
        sf::Vector2f buttonPosition(300, 200);

        for (int i = 0; i < 4; ++i) {
            sf::RectangleShape button(buttonSize);
            button.setPosition(buttonPosition.x, buttonPosition.y + i * 100);
            button.setFillColor(sf::Color(0,124,128));
            buttons.push_back(button);
        }
    }

   void run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);
            }
        }

        window.clear(sf::Color::White);

        // Draw buttons and their text
        for (const auto& button : buttons) {
            window.draw(button);
        }

        // Create text for the buttons
        sf::Font font;
        font.loadFromFile("fonts/times new roman.ttf");

        // Draw the text on the buttons
        for (int i = 0; i < buttons.size(); ++i) {
            sf::Text buttonText;
            buttonText.setFont(font); // Use default font
            if(i==0)
            {
                buttonText.setString("DJIKTRA'S");
            }
            else if(i==1)
            {
                buttonText.setString("DFS");
            }
            else if(i==2)
            {
                buttonText.setString("Greedy BEST FIRST");
            }
            else if(i==3)
            {
                buttonText.setString("A*");
            }
            buttonText.setCharacterSize(20); // Set text size
            buttonText.setFillColor(sf::Color::White); // Set text color
            buttonText.setStyle(sf::Text::Bold); // Set text style (bold)

            // Center the text within the button
            sf::FloatRect textBounds = buttonText.getLocalBounds();
            buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f,
                                 textBounds.top + textBounds.height / 2.0f);
            buttonText.setPosition(buttons[i].getPosition().x + buttons[i].getSize().x / 2.0f,
                                    buttons[i].getPosition().y + buttons[i].getSize().y / 2.0f);

            // Draw the text on the button
            window.draw(buttonText);
        }

        window.display();
    }
}


    void handleMouseClick(int x, int y) {
        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].getGlobalBounds().contains(x, y)) {
                handleButtonClick(i);
            }
        }
    }

    void handleButtonClick(size_t buttonIndex) {
        if (buttonIndex == 0) {
            // Djikstra's Algorithm button clicked, navigate to Pathfinding Visualizer
            PathfindingVisualizer visualizer;
            window.close(); // Close the main page window
        }
        if (buttonIndex == 1) {
            // DFS Algorithm button clicked, navigate to Pathfinding Visualizer
            PathfindingVisualizer1 visualizer;
            window.close(); // Close the main page window
        }
        if (buttonIndex == 2) {
            // greedy best fit Algorithm button clicked, navigate to Pathfinding Visualizer
            PathfindingVisualizer2 visualizer;
            window.close(); // Close the main page window
        }
        if (buttonIndex == 3) {
            // A* Algorithm button clicked, navigate to Pathfinding Visualizer
            PathfindingVisualizer3 visualizer;
            window.close(); // Close the main page window
        }
    }
};

int main() {
    MainVisualizerPage mainPage;
    return 0;
}
