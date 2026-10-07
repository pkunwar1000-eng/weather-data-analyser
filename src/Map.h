#ifndef ICT283_MAP_H
#define ICT283_MAP_H

#include <map>

	/**
	 * @class Map
	 * @brief A collection that stores values so each one can be found by a key.
	 *
	 * A Map holds pairs of a key and a value. The key is a label, and the value
	 * is whatever is being kept under that label. Given a key, the matching
	 * value can be found without looking through everything else.
	 *
	 * Keys are unique. Putting a value in under a key that is already there
	 * replaces the value that was there before, so there is never more than one
	 * value for the same key.
	 *
	 * This class wraps std::map. Wrapping means the std::map is a private
	 * member and nothing outside this file can see it or use it directly. The
	 * rest of the program only ever sees the methods below. This keeps all the
	 * STL detail in one place, so if the storage ever had to change to
	 * something else, only this file would need editing.
	 *
	 * A Map is known only by the methods below, so nothing that uses a Map is
	 * allowed to assume anything about how the values are actually stored.
	 *
	 * The public interface is kept to what a map is really for: putting a value
	 * in under a key, getting it back, asking whether a key is there, asking
	 * how many values are held, and going through everything that is stored.
	 * Nothing has been added just because it happened to be convenient for one
	 * particular program.
	 *
	 * The type used for K must have operator< so the map can order the keys.
	 * Both K and V must have a copy constructor, because the map stores its own
	 * copies of whatever is put into it.
	 *
	 * The constructor has an empty body. A std::map already starts empty, so
	 * there is nothing to do, but naming the member in the initialiser list
	 * says plainly that it was not forgotten. There is no destructor, because
	 * a std::map already frees itself.
	 *
	 * @author Prasanna Kunwar
	 * @version 01
	 * @date 22/07/2026 Prasanna Kunwar, Created for Assignment 2
	 */
template <class K, class V>
class Map
{
public:

		/**
		 * @brief The type of routine that ForEach can call back.
		 *
		 * This names a pointer to a routine that takes one key and its value,
		 * and returns nothing. Both come through as const references, so the
		 * routine can read them but cannot change what is stored.
		 *
		 * The key must stay const because the map keeps its keys in order. If
		 * a key could be changed while it was stored, it would be left sitting
		 * in the wrong place and could no longer be found.
		 */
	typedef void (*Visit)(const K& key, const V& value);

		/**
		 * @brief Makes an empty Map.
		 *
		 * @return void
		 */
	Map();												// management method

		/**
		 * @brief Stores a value under a key.
		 *
		 * If the key is already in the Map, the value stored under it is
		 * replaced, because a key can only have one value.
		 *
		 * @param key - the key to store the value under
		 * @param value - the value to store
		 * @return void
		 */
	void Insert(const K& key, const V& value);		// primary behaviour

		/**
		 * @brief Gets the value stored under a key.
		 *
		 * The value is copied into the parameter, so the caller cannot change
		 * what is stored in the Map by accident.
		 *
		 * If the key is not there, the parameter is left alone and false is
		 * returned. This lets the caller tell the difference between a key that
		 * was missing and a key whose value happened to be empty.
		 *
		 * @param key - the key to look for
		 * @param value - filled in with the stored value if the key was found
		 * @return bool - true if the key was found, false if not
		 */
	bool Find(const K& key, V& value) const;		// primary behaviour

		/**
		 * @brief Returns how many key and value pairs are stored.
		 *
		 * @return int - the number of pairs held
		 */
	int Size() const;								// query status

		/**
		 * @brief Calls the given routine once for every pair, in key order.
		 *
		 * This is how the whole Map is gone through without letting anything
		 * outside see how it is stored. The Map walks its own pairs and hands
		 * each one to the routine, and the caller decides what to do with them.
		 *
		 * Both the key and the value are handed over as const references, so
		 * the routine cannot change what is stored while the walk is going on.
		 *
		 * @param visit - the routine to call for each pair
		 * @return void
		 */
	void ForEach(Visit visit) const;					// primary behaviour

private:

		/// The pairs, kept in a std::map. Private, so nothing outside can
		/// reach the std::map or depend on it being a std::map.
	std::map<K, V> m_items;
};

// ---------------------------------------------------------------------------
// Implementation.
//
// Map is a template, so the implementation has to stay in this header file.
// The interface above is still kept separate from the code down here.
// ---------------------------------------------------------------------------

template <class K, class V>
Map<K, V>::Map()
	: m_items()
{
}

template <class K, class V>
void Map<K, V>::Insert(const K& key, const V& value)
{
	// Writing to the key replaces whatever was stored under it before, and
	// creates the entry if the key was not there yet.
	m_items[key] = value;
}

template <class K, class V>
bool Map<K, V>::Find(const K& key, V& value) const
{
	// find returns a position in the map. If the key is not there, that
	// position is end(), which means "past the last item".
	typename std::map<K, V>::const_iterator position = m_items.find(key);

	if(position == m_items.end())
	{
		return false;
	}

	// second is the value of the pair, first is the key.
	value = position->second;

	return true;
}

template <class K, class V>
int Map<K, V>::Size() const
{
	return static_cast<int>(m_items.size());
}

template <class K, class V>
void Map<K, V>::ForEach(Visit visit) const
{
	// If no routine was given there is nothing to call, so do nothing rather
	// than following a null pointer and crashing.
	if(visit == nullptr)
	{
		return;
	}

	// Walk every pair and hand it to the routine. A std::map keeps its keys in
	// order, so the pairs come out in key order.
	typename std::map<K, V>::const_iterator position;

	for(position = m_items.begin(); position != m_items.end(); ++position)
	{
		visit(position->first, position->second);
	}
}

#endif
