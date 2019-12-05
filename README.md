# Gragh
# 자료구조 시간에 구현한 그래프 자료구조입니다 ( 사용언어 : C )
# Prob1 폴더는 Gragh 자료구조를 인접행렬 & 인접리스트 방식으로 출력하도록 구성된 2가지 프로그램으로 이루어져 있습니다.
# 모두 Directed 방식으로 구현하였습니다. vertex를 추가하거나 삭제할 때, edge를 추가하거나 삭제할 때
# 삭제될 Object에 Link된 다른 vertex와 edge들이 길을 잃지 않도록 다시 잘 이어주는 과정이 매우 중요합니다.
# test 소스파일은 같이 있으므로 시뮬레이션 해보시면 됩니다.
#
# Prob2 폴더는 앞서 구현한 인접행렬 & 인접리스트 방식의 그래프를 활용해서 DFS와 BFS를 출력하는 프로그램인데
# 입력 방식을 콘솔 입력이 아닌 파일 입력으로 받고 출력은 콘솔에 출력되도록 했습니다.
# test용 텍스트 파일도 같이 첨부되어 있습니다.
#
# Prob3 폴더는 인접리스트 방식의 Directed Gragh 의 edge에 Weight(가중치)를 추가해 구현한 프로그램입니다
# Weight 그래프의 경우 최단 거리를 구하는 데 활용될 수 있기 때문에 이러한 작업을 하게 되었습니다.
# 크게 달라지는 건 없고 edge 구조체에서 weight 의 멤버 변수가 추가되었고 그래프 정보 출력 시 Weight까지 출력됩니다.
# test용 소스파일도 같이 있습니다.
#
# Prob4 폴더는 Prob3 폴더의 인접리스트 방식 Directed Weight Gragh 를 활용한 Applications입니다.
# BFS와 DFS이 구현되어 있고, Minimum Spanning Tree를 구현하였습니다.
# MST는 Gragh의 모든 노드를 포함하는 트리 중에서도 (Spanning Tree) 
# 트리의 모든 edge들의 weight의 합이 최소인 트리를 말합니다.
# 널리 알려져 있는 Prim's algorithm을 활용합니다.
# 추가로 시작지점에서 각 지점으로의 최단 거리를 구하는 것도 구현되어 있습니다.
# 다익스트라 알고리즘을 활용합니다.
# 파일 입력으로 구현하였습니다. ( Test파일 첨부 )
# (참고: 시작지점은 A로 되어 있습니다.)
