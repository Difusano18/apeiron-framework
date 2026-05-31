#include <catch2/catch_test_macros.hpp>

#include <apeiron/distributed/cluster_manager.h>
#include <apeiron/distributed/data_synchronizer.h>
#include <apeiron/distributed/message_broker.h>
#include <apeiron/distributed/task_scheduler.h>

using namespace apeiron::distributed;

TEST_CASE("ClusterManager registers and filters healthy nodes", "[distributed][cluster]") {
    ClusterManager cluster;
    cluster.register_node(ClusterNode{"a", "local-a", true});
    cluster.register_node(ClusterNode{"b", "local-b", true});

    REQUIRE(cluster.mark_unhealthy("b"));
    auto healthy = cluster.healthy_nodes();
    REQUIRE(healthy.size() == 1);
    REQUIRE(healthy.front().id == "a");
}

TEST_CASE("MessageBroker queues messages in process", "[distributed][messages]") {
    MessageBroker broker;
    broker.publish(Message{"a", "b", "tick", "1"});

    Message out;
    REQUIRE(broker.try_consume(out));
    REQUIRE(out.topic == "tick");
    REQUIRE_FALSE(broker.try_consume(out));
}

TEST_CASE("TaskScheduler schedules local task descriptors", "[distributed][tasks]") {
    TaskScheduler scheduler;
    auto task = scheduler.schedule("node-a", "think", 5);

    REQUIRE(task.id == 1);

    ScheduledTask out;
    REQUIRE(scheduler.try_next(out));
    REQUIRE(out.name == "think");
}

TEST_CASE("DataSynchronizer stores versioned records", "[distributed][sync]") {
    DataSynchronizer sync;
    auto first = sync.update("memory", "a");
    auto second = sync.update("memory", "b");

    REQUIRE(first.version == 1);
    REQUIRE(second.version == 2);

    SyncRecord out;
    REQUIRE(sync.get("memory", out));
    REQUIRE(out.value == "b");
}
