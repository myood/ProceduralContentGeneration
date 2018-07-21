#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>

namespace SpacePartition
{

struct area_tag { using kind = boost::vertex_property_tag; };
struct area_t
{ 
    int top, left, bottom, right; 
    uint width() const;
    uint height() const;
};
using Areas = std::vector<area_t>;
using roomsProperty = boost::property<area_tag, area_t>;
using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, roomsProperty>;
using GraphRooms = boost::property_map<Graph, area_tag>::type;

using Node = boost::graph_traits<Graph>::vertex_descriptor;
using Nodes = std::vector<SpacePartition::Node>;

inline bool operator==(const SpacePartition::area_t &lhs, const SpacePartition::area_t &rhs)
{
    return lhs.top == rhs.top and lhs.bottom == rhs.bottom and lhs.left == rhs.left and lhs.right == rhs.right;
}

inline ::std::ostream &operator<<(::std::ostream &os, const SpacePartition::area_t &area)
{
    return os << "[top: " << area.top << ", left: " << area.left << ", bottom: " << area.bottom << ", right: " << area.right << "]";
}

}  // namespace SpacePartition