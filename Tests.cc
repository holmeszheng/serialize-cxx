#define CATCH_CONFIG_MAIN
#include "catch.hpp"


#include "Structures.h"
#include "Serializer.h"
#include "Deserializer.h"


TEST_CASE("initialization", "[Node]") {
    Node node;

    CHECK(node.prev == nullptr);
    CHECK(node.next == nullptr);
    CHECK(node.data == nullptr);
    CHECK(node.dataCount == 0);
    CHECK(node.name == std::string());
}

TEST_CASE("simpleRoundtrip", "[Serializer]") {
    Node node;
    node.name = "Hello World";

    uint32_t size = 0;
    uint8_t* serialized = Serializer::serialize(&node, &size);

    REQUIRE_FALSE(serialized == nullptr);
    REQUIRE_FALSE(reinterpret_cast<void *>(serialized) == reinterpret_cast<void *>(&node));
    REQUIRE(size > 0);

    Node *result = Deserializer::deserialize(serialized, size);

    REQUIRE_FALSE(result == nullptr);
    REQUIRE_FALSE(result == &node);
    CHECK(result->name == node.name);
}

TEST_CASE("dataRoundTrip", "[Serializer]") {
    // This has to be dynamically allocated because of the node destructor.
    Data *data = static_cast<Data *>(malloc(sizeof(Data) * 2));

    data[0].id = 5;
    data[0].x = 10.f;
    data[0].y = -10.f;

    data[1].id = 2;
    data[1].x = 0.f;
    data[1].y = -1.f;

    Node node;
    node.data = &data[0];
    node.dataCount = 2;

    uint32_t size = 0;
    uint8_t* serialized = Serializer::serialize(&node, &size);

    REQUIRE_FALSE(serialized == nullptr);
    REQUIRE(size > 0);

    Node *result = Deserializer::deserialize(serialized, size);

    REQUIRE_FALSE(result == nullptr);

    // Verify the data contents
    REQUIRE(result->dataCount == 2);
    REQUIRE_FALSE(result->data == nullptr);

    Data *d0 = result->data;
    REQUIRE_FALSE(&data[0] == d0);

    Data *d1 = result->data + 1;
    REQUIRE_FALSE(&data[1] == d1);

    CHECK(d0->id == data[0].id);
    CHECK(d0->x == data[0].x);
    CHECK(d0->y == data[0].y);

    CHECK(d1->id == data[1].id);
    CHECK(d1->x == data[1].x);
    CHECK(d1->y == data[1].y);
}

TEST_CASE("linkedRoundTrip", "[Serializer]") {
    Node n0;
    n0.name = "first";

    Node n1;
    n1.name = "second";

    Node n2;
    n2.name = "third";

    n0.next = &n1;
    n1.next = &n2;

    n2.prev = &n1;
    n1.prev = &n0;

    uint32_t size = 0;
    uint8_t* serialized = Serializer::serialize(&n0, &size);

    REQUIRE_FALSE(serialized == nullptr);
    REQUIRE(size > 0);

    Node *result = Deserializer::deserialize(serialized, size);

    REQUIRE_FALSE(result == nullptr);

    CHECK(result->name == n0.name);
    CHECK(result->prev == nullptr);

    REQUIRE_FALSE(result->next == nullptr);
    CHECK(result->next->name == n1.name);
    CHECK(result->next->prev == result);

    REQUIRE_FALSE(result->next->next == nullptr);
    CHECK(result->next->next->name == n2.name);
    CHECK(result->next->next->prev == result->next);
    CHECK(result->next->next->next == nullptr);
}

TEST_CASE("cycleRoundTrip", "[Serializer]") {
    Node n0;
    n0.name = "This is the song that never ends";

    Node n1;
    n1.name = "It just goes on and on my friend";

    Node n2;
    n2.name = "Some people started singing it not knowing what it was";

    Node n3;
    n3.name = "And they'll continue singing it for ever just because";

    n0.next = &n1;
    n0.prev = &n3;

    n1.next = &n2;
    n1.prev = &n0;

    n2.next = &n3;
    n2.prev = &n1;

    n3.next = &n0;
    n3.prev = &n2;

    uint32_t size = 0;
    uint8_t* serialized = Serializer::serialize(&n0, &size);

    REQUIRE_FALSE(serialized == nullptr);
    REQUIRE(size > 0);

    Node *r0 = Deserializer::deserialize(serialized, size);

    REQUIRE_FALSE(r0 == nullptr);
    REQUIRE_FALSE(r0->next == nullptr);

    Node *r1 = r0->next;
    REQUIRE_FALSE(r1->next == nullptr);

    Node *r2 = r1->next;
    REQUIRE_FALSE(r2->next == nullptr);

    Node *r3 = r2->next;
    CHECK(r3->next == r0);

    CHECK(r0->prev == r3);
    CHECK(r1->prev == r0);
    CHECK(r2->prev == r1);
    CHECK(r3->prev == r2);

    CHECK(n0.name == r0->name);
    CHECK(n1.name == r1->name);
    CHECK(n2.name == r2->name);
    CHECK(n3.name == r3->name);

}
