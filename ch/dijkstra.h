#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/pending/relaxed_heap.hpp>
//Foncteur annexe qui retourne vrai si le 1er nœud a une distance plus petite
template<int N>
struct Compare
{
    const std::vector<float> & vec;
    Compare(const std::vector<float> & dist) : vec(dist) {}
    bool operator()(const typename Graph<N>::node_t a, const typename Graph<N>::node_t b) const
    {
        return vec[a] < vec[b];
    }
};

enum Color {White, Gray, Black};

//Simple fonction pour tester si le calcul du plus court chemin fonctionne
//On retourne que la distance pour comparer par rapport à du dijkstra
template<int N>
float query_mono(typename Graph<N>::node_t start, typename Graph<N>::node_t dest, const typename Graph<N>::Type & graph)
{
        std::vector<int> p(boost::num_vertices(graph));
        std::vector<float> d(boost::num_vertices(graph));
        boost::dijkstra_shortest_paths(graph, start, boost::predecessor_map(&p[0]).distance_map(&d[0]).weight_map(get(&Graph<2>::Edge::cost0, graph)));


    //Vecteurs de distance
    std::vector<float> dist1(boost::num_vertices(graph), std::numeric_limits<float>::max());
    std::vector<float> dist2(boost::num_vertices(graph), std::numeric_limits<float>::max());

    //Les deux files de priorité
    boost::relaxed_heap<typename Graph<N>::node_t, Compare<N> > queue1(boost::num_vertices(graph), Compare<N>(dist1));
    boost::relaxed_heap<typename Graph<N>::node_t, Compare<N> > queue2(boost::num_vertices(graph), Compare<N>(dist2));

    //La couleur du nœud (blanc : non exploré, gris : touché, noir : on y touche plus)
    std::vector<Color> col1(boost::num_vertices(graph), White);
    std::vector<Color> col2(boost::num_vertices(graph), White);

    dist1[start] = 0;
    col1[start] = Gray;
    queue1.push(start);
    
    dist2[dest] = 0;
    col2[dest] = Gray;
    queue2.push(dest);

    float best_distance = std::numeric_limits<float>::max();
    while(!queue1.empty() || !queue2.empty())
    {
        if(!queue1.empty())
        {
            //On prend le plus petit nœud gris
            typename Graph<N>::node_t n1 = queue1.top();
            if(dist1[n1] > d[n1] + 0.01)
            {
     //           std::cout << "We have pb at node " << n1 << " " << dist1[n1] << " vs " << d[n1] << std::endl;
            }
            queue1.pop();
            col1[n1] = Black;
            //Pour tous ses successeurs
            BOOST_FOREACH(typename Graph<N>::edge_t edge, boost::out_edges(n1, graph))
            {
                typename Graph<N>::node_t v = boost::target(edge, graph);
                //On ne prend que les nœuds plus grands
                if(graph[v].order >= graph[n1].order)
                {
                    BOOST_ASSERT(graph[edge].cost0 > 0);
                    //Si on a trouvé plus court vers le successeurs
                    if(dist1[n1] + graph[edge].cost0 < dist1[v])
                    {
                        dist1[v] = dist1[n1] + graph[edge].cost0;
                        //Si on avait jamais visité ce nœud, on le rajoute à la queue
                        if(col1[v] == White)
                        {
                            col1[v] = Gray;
                            queue1.push(v);
                        }
                        else if(col1[v] == Gray)
                        {
                            queue1.update(v);
                        }
                    }
                }
                //On fait un check des fois que le nœud ait déjà été atteint
                // dans l'autre sens
                if(col2[v] != White && dist1[v] + dist2[v] < best_distance)
                {
                    best_distance = dist1[v] + dist2[v];
                }
            }
        }

        // HOP ! Copier-coller pour avoir l'algo qui tourne dans l'autre sens
        if(!queue2.empty())
        {
            //On prend le plus petit nœud gris
            typename Graph<N>::node_t n2 = queue2.top();
            queue2.pop();
            //Pour tous ses successeurs
            BOOST_FOREACH(typename Graph<N>::edge_t edge, boost::in_edges(n2, graph))
            {
                typename Graph<N>::node_t v = boost::source(edge, graph);
                //On ne prend que les nœuds plus grands
                if(graph[v].order >= graph[n2].order)
                {
                    //Si on a trouvé plus court vers le successeurs
                    if(dist2[n2] + graph[edge].cost0 < dist2[v])
                    {
                        dist2[v] = dist2[n2] + graph[edge].cost0;
                        //Si on avait jamais visité ce nœud, on le rajoute à la queue
                        if(col2[v] == White)
                        {
                            col2[v] = Gray;
                            queue2.push(v);
                        }
                        else if(col2[v] == Gray)
                        {
                            queue2.update(v);
                        }
                    }
                }
                //On fait un check des fois que le nœud ait déjà été atteint
                if(col1[v] != White && dist1[v] + dist2[v] < best_distance)
                {
                    best_distance = dist1[v] + dist2[v];
                }
            }
            col2[n2] = Black;
        }
    }

    return best_distance;
}

#endif
