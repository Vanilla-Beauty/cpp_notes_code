# Compiler settings
CXX := g++-14
CXXFLAGS := -std=c++11 -Wall -Wextra

# ASIO include path (adjust as needed)
ASIO_INCLUDE := /home/toni/proj/asio-1.30.2/include

# OpenSSL libraries (required for HTTPS)
LDFLAGS := -lssl -lcrypto

# Source files
SRC_HTTP := sync/sync_http_client.cpp
SRC_HTTPS := sync/sync_https_client.cpp

SRC_TIMER := async/timer.cpp
SRC_AHTTPS := async/async_https_client.cpp

# Output executables
TARGET_HTTP := sync_http_client
TARGET_HTTPS := sync_https_client
TARGET_TIMER := timer
TARGET_AHTTPS := async_https_client


# Default target (build both HTTP and HTTPS)
all: $(TARGET_HTTP) $(TARGET_HTTPS)

# HTTP target
$(TARGET_HTTP): $(SRC_HTTP)
	$(CXX) $(CXXFLAGS) -I$(ASIO_INCLUDE) $< -o $@

# HTTPS target (requires OpenSSL)
$(TARGET_HTTPS): $(SRC_HTTPS)
	$(CXX) $(CXXFLAGS) -I$(ASIO_INCLUDE) $< $(LDFLAGS) -o $@

$(TARGET_TIMER): $(SRC_TIMER)
	$(CXX) $(CXXFLAGS) -I$(ASIO_INCLUDE) $< -o $@

$(TARGET_AHTTPS): $(SRC_AHTTPS)
	$(CXX) $(CXXFLAGS) -I$(ASIO_INCLUDE) $< $(LDFLAGS) -o $@

# Clean build artifacts

all: $(TARGET_HTTP) $(TARGET_HTTPS) $(TARGET_TIMER) $(TARGET_AHTTPS)


clean:
	rm -f $(TARGET_HTTP) $(TARGET_HTTPS) $(TARGET_TIMER) $(TARGET_AHTTPS)

.PHONY: all clean