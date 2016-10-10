cmake_minimum_required(VERSION 3.3)
project(cafca)

include_directories(src)

find_package(Boost COMPONENTS iostreams filesystem system regex REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(
        SOURCE_FILES
        src/cafca/main.cc
        src/cafca/util/byte_buffer.cc src/cafca/util/byte_buffer.h
        src/cafca/commit_log/offset_index.cc src/cafca/commit_log/offset_index.h
        src/cafca/commit_log/commit_log_file.cc src/cafca/commit_log/commit_log_file.h
        src/cafca/util/file_channel.cc src/cafca/util/file_channel.h
        src/cafca/message/message.cc src/cafca/message/message.h
        src/cafca/message/messages_buffer.cc src/cafca/message/messages_buffer.h
        src/cafca/message/message_set.cc src/cafca/message/message_set.h
        src/cafca/util/binary.h
        src/cafca/commit_log/commit_log_segment.cc src/cafca/commit_log/commit_log_segment.h
        src/cafca/commit_log/commit_log.cc src/cafca/commit_log/commit_log.h
        src/cafca/commit_log/commit_log_offset_metadata.h
        src/cafca/server/api/socket_server.cc src/cafca/server/api/socket_server.h
        src/cafca/server/cafca_server.cc src/cafca/server/cafca_server.h
        src/cafca/server/api/connection.cc src/cafca/server/api/connection.h
        src/cafca/server/api/command/producer_request.cc src/cafca/server/api/command/producer_request.h
        src/cafca/server/cluster/topic_and_partition.h
        src/cafca/server/cluster/replica.h
        src/cafca/server/cluster/partition.cc src/cafca/server/cluster/partition.h
        src/cafca/server/api/command/producer_response.cc src/cafca/server/api/command/producer_response.h
        src/cafca/server/cafca_components.h
        src/cafca/server/api/command/topic_metadata_request.cc src/cafca/server/api/command/topic_metadata_request.h
        src/cafca/server/api/command/topic_metadata_response.cc src/cafca/server/api/command/topic_metadata_response.h
        src/cafca/server/cluster/broker_endpoint.h
        src/cafca/util/short_string.h
        src/cafca/util/optional.h
        src/cafca/server/cluster/topic_metadata.h
        src/cafca/server/cluster/partition_members.h src/cafca/server/cluster/active_members.h
        src/cafca/server/cafca_controller.h src/cafca/server/cafca_controller.cc
        src/cafca/server/api/command/offset_request.cc src/cafca/server/api/command/offset_request.h
        src/cafca/server/api/command/offset_response.cc src/cafca/server/api/command/offset_response.h
        src/cafca/message/file_message_set.h
        src/cafca/server/api/command/partition_fetch_info.h
        src/cafca/server/api/command/fetch_request.cc src/cafca/server/api/command/fetch_request.h
        src/cafca/server/api/command/fetch_response.cc src/cafca/server/api/command/fetch_response.h
        src/cafca/util/logging.h)

add_executable(cafca ${SOURCE_FILES} src/cafca/main.cc)

target_link_libraries(cafca ${Boost_LIBRARIES})
