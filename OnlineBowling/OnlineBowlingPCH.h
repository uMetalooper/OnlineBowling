#define ALLEY_WIDTH (1.0f) // 1 meter wide
#define HALF_ALLEY_WIDTH (0.5f)
#define ALLEY_LENGTH (18.288f) // 18.288 meters long
#define BALL_RADIUS (0.1f)

// Common
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Networking
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
typedef int socklen_t;

#include "memory"

#include "vector"
#include "unordered_map"
#include "list"
#include "queue"
#include "deque"
#include "unordered_set"
#include "cassert"

using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::queue;
using std::list;
using std::deque;
using std::unordered_map;
using std::string;
using std::unordered_set;

class Ball;
class GameObject;

#include "StringUtils.h"
#include "TinyLogger.h"

#include "SocketAddress.h"
#include "SocketAddressFactory.h"
#include "UDPSocket.h"
#include "TCPSocket.h"
#include "SocketUtil.h"

#include "MemoryBitStream.h"

#include "InputAction.h"
#include "InputState.h"
#include "Move.h"
#include "MoveList.h"

#include "InputManager.h"
#include "Timing.h"

#include "GameObject.h"

#include "GameObjectRegistry.h"
#include "World.h"
#include "Floor.h"
#include "Ball.h"
#include "ScoreBoardManager.h"

#include "ResourcesManager.h"

#include "WeightedTimedMovingAverage.h"
#include "ReplicationCommand.h"
#include "NetworkManager.h"
#include "Engine.h"


// Rendering
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/ext.hpp"
typedef glm::vec3 Vector3;


#include "Camera.h"


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

