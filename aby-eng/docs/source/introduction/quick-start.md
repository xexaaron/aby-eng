# Quick Start

## Editorless

Setting up the project build file with cmake

If you want to use Qt in your project then refer to the QT
specific sections for more information.

### CMake Configuration

???+ example "CMake Configuration"

    ```cmake
    cmake_minimum_required(VERSION 3.28.3)
    project(NAME LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    add_executable(${PROJECT_NAME}
        # source files
    )

    target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
    )

    target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        aby-eng::aby-eng
    )

    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:
            /WX              # warnings as errors
            /Zc:preprocessor # for logging macros using __VA_OPT__
        > 
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror> # warnings as errors
    )

    # everything on msvc uses this specific runtime library
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
    ```

    ??? note "Qt window backend"
        For using the Qt windowing backend, enable Qt's automatic build tools:

        ```cmake
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTORCC ON)
        find_package(Qt6 REQUIRED COMPONENTS Widgets)
        ```
        Then link against `Qt6::Widgets` and use `qt_add_executable` instead of `add_executable`.

### The Entry Point

???+ example "Setting up the entry point"

    ???+ example "main.hpp"
        ```cpp
        #include <aby-eng/core/entry.hpp>

        class EntryPoint final : public aby::eng::EntryPoint {
        public:
            EntryPoint(int argc, char** argv);

            auto on_cmdl(argparse::ArgumentParser& parser) -> void override;
            auto on_exec() -> void override;
            auto on_create() -> void override;
            auto on_exit() -> void override;
        };
        ```
    
    ???+ example "main.cpp"
        ```cpp
        EntryPoint::EntryPoint(int argc, char** argv) :
            aby::eng::EntryPoint({
                .name    = "PROJECT_NAME",
                .version = "PROJECT_VERSION",
                .argc    = argc,
                .argv    = argv,
                .win_cfg = win::Config{
                                    .name           = "WINDOW_TITLE",
                                    .width          = 800,
                                    .height         = 600,
                                    .window_backend = win::EWindow::sdl, // or qt, or glfw
                                    .render_backend = win::ERenderBackend::vulkan,
                                    }
        }) {
        }

        auto EntryPoint::on_cmdl(argparse::ArgumentParser& parser) -> void {
            // Use this to register cmdline arguments and store them into variables when parsed
            // parser.add_argument(...)
            //       ...
            //       .store_into(my_variable);
            // the variable will be populated
            // by the time on_exec is called
        }

        auto EntryPoint::on_exec() -> void {
            // Use this after systems have been initialzied to 
            // do configuration work, use variables that have been
            // parsed from the cmdline, etc. 
        }

        auto EntryPoint::on_create() -> void {
            // This is the time to create objects and entities.
        }

        auto EntryPoint::on_exit() -> void {
            // Cleanup any extra resources you possess
        }

        // Define the main function and use the entry point
        int main(int argc, char** argv) {
            return EntryPoint::exec<EntryPoint>(argc, argv);
        }

        ```

    ??? note "Qt window backend"
        For the qt window backend the project will create a QMainWindow or
        QWindow based on the `child` flag in the `win::config`.
    
        You must construct a QApplication inside your entry point constructor so that QT can create a window. 

        ```cpp
        // This is the configuration that the editor uses
        // The editor itself creates a QMainWindow and then embeds the 
        // engines window into it. 
        EntryPoint::EntryPoint(int argc, char** argv) :
            eng::EntryPoint({
                .name    = "PROJECT_NAME",
                .version = "PROJECT_VERSION",
                .argc    = argc,
                .argv    = argv,
                .win_cfg = win::Config{
                                    .name           = "WINDOW_TITLE",
                                    .width          = 800,
                                    .height         = 600,
                                    .child          = true,
                                    .window_backend = win::EWindow::qt,
                                    .render_backend = win::ERenderBackend::vulkan,
                                    }
        }),
            // Use the m_AppInfo variables as QT requires references to
            // the argc and argv of the application
            m_App(new QApplication(m_AppInfo.argc, m_AppInfo.argv)) {
        }
        ```

        When using the engine to create a QWindow versus a QMainWindow
        so that it can be embeded into a QMainWindow then it is required
        to properly handle incoming events for the QT window by installing
        an event filter. 

        ```cpp
        class ParentCloseFilter final : public QObject {
        public:
            ParentCloseFilter(aby::win::Window* window, QObject* parent = nullptr);
        protected:
            bool eventFilter(QObject*, QEvent* event) override;
        private:
            aby::win::Window* m_Window;
        };

        ParentCloseFilter::ParentCloseFilter(win::Window* window, QObject* parent) :
            QObject(parent),
            m_Window(window) {
        }

        bool ParentCloseFilter::eventFilter(QObject*, QEvent* event) {
            if (event->type() == QEvent::Close) {
                m_Window->close();
            }
            return false;
        }
        ```
        Somewhere in your project setup code, ie `EntryPoint::on_exec`

        ```cpp
        auto* main_window = GET_YOUR_QMAINWINDOW();
        
        auto viewport_window = App::window();
        auto* viewport = static_cast<QWindow*>(viewport_window->native().backend_window);
        
        main_window->installEventFilter(new ParentCloseFilter(viewport_window, this));
		
        auto* render_widget = QWidget::createWindowContainer(viewport, this);
		
        render_widget->setFocusPolicy(Qt::StrongFocus);
		setCentralWidget(render_widget);
        ```

## Editor

<!-- TODO: Write section -->

!!! warning
    TODO: Write section