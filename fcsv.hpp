#pragma once

#include <array>
#include <istream>
#include <sstream>
#include <tuple>
#include <vector>

namespace fcsv::detail {

template <typename... Ts>
using Record = std::tuple<Ts...>;

template <std::size_t N>
using Row = std::array<std::string, N>;

template <std::size_t N, typename... Ts>
using Nth = std::tuple_element_t<N, std::tuple<Ts...>>;

template <typename T>
auto to(const std::string& s) -> T
{
    return s;
}

template <>
auto to<int>(const std::string& s) -> int
{
    return std::stoi(s);
}

template <>
auto to<long>(const std::string& s) -> long
{
    return std::stol(s);
}

template <>
auto to<long long>(const std::string& s) -> long long
{
    return std::stoll(s);
}

template <>
auto to<unsigned>(const std::string& s) -> unsigned
{
    return std::stoul(s);
}

template <>
auto to<unsigned long>(const std::string& s) -> unsigned long
{
    return std::stoul(s);
}

template <>
auto to<unsigned long long>(const std::string& s) -> unsigned long long
{
    return std::stoull(s);
}

template <>
auto to<float>(const std::string& s) -> float
{
    return std::stof(s);
}

template <>
auto to<double>(const std::string& s) -> double
{
    return std::stod(s);
}

template <typename T>
void assign(T& t, const T& val)
{
    t = val;
}

template <typename... Ts, std::size_t... Is>
auto read(const Row<sizeof...(Ts)>& r, std::index_sequence<Is...>) -> Record<Ts...>
{
    Record<Ts...> res;
    (assign(std::get<Is>(res), to<Nth<Is, Ts...>>(r.at(Is))), ...);

    return res;
}

template <typename... Ts>
auto read(const Row<sizeof...(Ts)>& r) -> Record<Ts...>
{
    return read<Ts...>(r, std::index_sequence_for<Ts...>{});
}

template <std::size_t N>
auto split(const std::string& s) -> Row<N>
{
    Row<N> res{};
    std::istringstream stream{s};

    for (unsigned i = 0; i < N; ++i)
        std::getline(stream, res[i], ',');

    return res;
}

template <typename... Ts, std::size_t... Is>
auto to_string(const Record<Ts...>& r, std::index_sequence<Is...>) -> std::string
{
    std::stringstream s;
    ((s << (Is ? "," : "") << std::get<Is>(r)) , ...);

    return s.str();
}

} // namespace fcsv::detail


namespace fcsv {

template <typename... Ts>
using Record = std::tuple<Ts...>;

template <typename... Ts>
auto read(const std::string& s) -> Record<Ts...>
{
    return detail::read<Ts...>(detail::split<sizeof...(Ts)>(s));
}

template <typename... Ts>
auto read(std::istream& s) -> std::vector<Record<Ts...>>
{
    std::vector<Record<Ts...>> res;
    res.reserve(1000);

    std::string line;
    while (std::getline(s, line))
    {
        if (line.empty())
            continue;

        res.push_back(read<Ts...>(line));
    }

    return res;
}

template <typename... Ts>
auto to_string(const Record<Ts...>& r) -> std::string
{
    return detail::to_string(r, std::index_sequence_for<Ts...>{});
}

template <typename... Ts>
void write(const Record<Ts...>& r, std::ostream& os)
{
    os << to_string(r) << std::endl;
}

template <typename... Ts>
void write(const std::vector<Record<Ts...>>& records, std::ostream& os)
{
    for (const auto& r : records)
        write(r, os);
}

} // namespace fcsv

namespace fcsv2::detail {


} // namespace fcsv2::detail

namespace fcsv2 {

template <typename... Ts>
struct Reader
{
    using Record = std::tuple<Ts...>;

    template <std::size_t N>
    using Nth = std::tuple_element_t<N, std::tuple<Ts...>>;

    Reader(std::istream& s) : m_stream{s} {}

    void split()
    {
        std::string line;
        std::getline(m_stream, line);

        std::istringstream s{std::move(line)};

        for (unsigned i = 0; i < m_split.size(); ++i)
            std::getline(s, m_split[i], ',');
    }

    auto next() -> std::tuple<Ts...>
    {
        split();
        return parse(std::index_sequence_for<Ts...>{});
    }

    template <typename T>
    void assign(T& t, const T& val)
    {
        t = val;
    }

    template <std::size_t... Is>
    auto parse(std::index_sequence<Is...>) -> Record
    {
        Record res;
        (assign(std::get<Is>(res), fcsv::detail::to<Nth<Is>>(m_split.at(Is))), ...);

        return res;
    }

    std::istream& m_stream;
    std::array<std::string, sizeof...(Ts)> m_split;
};

} // namespace fcsv2