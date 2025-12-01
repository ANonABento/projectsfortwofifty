#!/bin/bash

echo "=== File Loading & Tricky Edge Cases ==="
make clean && make

if [ ! -f "a.out" ]; then
    echo "Compilation failed!"
    exit 1
fi

# Create test entity file
cat > test_entities.txt << 'EOF'
node1 Alice student
node2 Bob teacher
node3 Carol student
EOF

# Create test relationship file
cat > test_relationships.txt << 'EOF'
node1 knows node2 5.0
node2 teaches node3 10.0
node1 friends node3 3.0
EOF

# Test O: Load files
echo ""
echo "=== TEST O: Load Files ===
"
cat > temp_test.in << 'EOF'
LOAD test_entities.txt entities
LOAD test_relationships.txt relationships
PRINT node1
PRINT node2
PATH node1 node3
EXIT
EOF

echo "Running:"
cat temp_test.in
echo ""
echo "Output:"
./a.out < temp_test.in
echo ""
echo "Expected:"
echo "success (load entities)"
echo "success (load relationships)"
echo "node2 node3 (neighbors of node1, sorted)"
echo "node1 node3 (neighbors of node2, sorted)"
echo "node1 node2 node3 15 (path through node2, weight 5+10=15)"
echo "or node1 node3 3 (direct path, weight 3 - but this is LOWER so should NOT be chosen)"
echo "---"

# Create entities with duplicate IDs
cat > test_dup_entities.txt << 'EOF'
id1 FirstName type1
id1 SecondName type2
id2 Another type3
EOF

# Test P: Load with duplicate IDs (should update)
echo ""
echo "=== TEST P: Load Duplicate IDs (Update) ==="
cat > temp_test.in << 'EOF'
LOAD test_dup_entities.txt entities
FINDALL name FirstName
FINDALL name SecondName
FINDALL type type1
FINDALL type type2
EXIT
EOF

echo "Running:"
cat temp_test.in
echo ""
echo "Output:"
./a.out < temp_test.in
echo ""
echo "Expected:"
echo "failure (FirstName was overwritten)"
echo "id1 (SecondName is current)"
echo "failure (type1 was overwritten)"
echo "id1 (type2 is current)"
echo "---"

# Create relationship file with missing nodes
cat > test_bad_rel.txt << 'EOF'
a edge b 1.0
x edge y 2.0
EOF

# Test Q: Load relationships with missing nodes (should skip)
echo ""
echo "=== TEST Q: Load Relationships with Missing Nodes ==="
cat > temp_test.in << 'EOF'
ENTITY a A t
ENTITY b B t
LOAD test_bad_rel.txt relationships
PRINT a
PATH x y
EXIT
EOF

echo "Running:"
cat temp_test.in
echo ""
echo "Output:"
./a.out < temp_test.in
echo ""
echo "Expected:"
echo "success, success, success"
echo "b (a->b edge was created)"
echo "failure (x and y don't exist, edge was skipped)"
echo "---"

# Test R: HIGHEST with tie-breaker scenario
echo ""
echo "=== TEST R: HIGHEST with Multiple Equal Paths ==="
cat > temp_test.in << 'EOF'
ENTITY a A t
ENTITY b B t
ENTITY c C t
ENTITY d D t
RELATIONSHIP a r b 5.0
RELATIONSHIP c r d 5.0
HIGHEST
EXIT
EOF

echo "Running:"
cat temp_test.in
echo ""
echo "Output:"
./a.out < temp_test.in
echo ""
echo "Expected: Either 'a b 5' or 'c d 5' (both valid, same weight)"
echo "---"

# Test S: Long alphanumeric IDs
echo ""
echo "=== TEST S: Long Alphanumeric IDs ==="
cat > temp_test.in << 'EOF'
ENTITY abc123XYZ789 LongID type1
ENTITY node2 Name2 type1
RELATIONSHIP abc123XYZ789 edge node2 2.5
PRINT abc123XYZ789
PATH abc123XYZ789 node2
EXIT
EOF

echo "Running:"
cat temp_test.in
echo ""
echo "Output:"
./a.out < temp_test.in
echo ""
echo "Expected:"
echo "success, success, success"
echo "node2"
echo "abc123XYZ789 node2 2.5"
echo "---"

# Test T: Mixed case IDs
echo ""
echo "=== TEST T: Mixed Case IDs ==="
cat > temp_test.in << 'EOF'
ENTITY aBc Name type
ENTITY AbC Name2 type
PRINT aBc
PRINT AbC
EXIT
EOF

echo "Running:"
cat temp_test.in
echo ""
echo "Output:"
./a.out < temp_test.in
echo ""
echo "Expected:"
echo "success, success"
echo "(blank - aBc has no edges)"
echo "(blank - AbC has no edges)"
echo "These are DIFFERENT nodes (case sensitive)"
echo "---"

# Test U: Empty name or type (edge case from file)
cat > test_edge_entity.txt << 'EOF'
id1 name1 type1
id2 name2 type2
EOF

echo ""
echo "=== TEST U: Normal File Load ==="
cat > temp_test.in << 'EOF'
LOAD test_edge_entity.txt entities
FINDALL type type1
FINDALL name name2
EXIT
EOF

echo "Running:"
cat temp_test.in
echo ""
echo "Output:"
./a.out < temp_test.in
echo ""
echo "Expected:"
echo "success"
echo "id1"
echo "id2"
echo "---"

# Test V: PATH after DELETE creates disconnection
echo ""
echo "=== TEST V: PATH After DELETE Disconnects Graph ==="
cat > temp_test.in << 'EOF'
ENTITY a A t
ENTITY b B t
ENTITY c C t
RELATIONSHIP a r b 1.0
RELATIONSHIP b r c 1.0
PATH a c
DELETE b
PATH a c
EXIT
EOF

echo "Running:"
cat temp_test.in
echo ""
echo "Output:"
./a.out < temp_test.in
echo ""
echo "Expected:"
echo "a b c 2 (path exists through b)"
echo "success (delete b)"
echo "failure (a and c now disconnected)"
echo "---"

# Test W: HIGHEST after operations
echo ""
echo "=== TEST W: HIGHEST After Multiple Operations ==="
cat > temp_test.in << 'EOF'
ENTITY a A t
ENTITY b B t
ENTITY c C t
RELATIONSHIP a r b 10.0
RELATIONSHIP b r c 5.0
HIGHEST
RELATIONSHIP a r c 20.0
HIGHEST
EXIT
EOF

echo "Running:"
cat temp_test.in
echo ""
echo "Output:"
./a.out < temp_test.in
echo ""
echo "Expected:"
echo "First HIGHEST: a c 15 (or c a 15) - path through b"
echo "After adding direct a-c edge with weight 20:"
echo "Second HIGHEST: a c 20 (or c a 20) - direct path is now better"
echo "---"

# Cleanup
rm -f temp_test.in test_entities.txt test_relationships.txt test_dup_entities.txt test_bad_rel.txt test_edge_entity.txt

echo ""
echo "=== File Loading Tests Complete ==="
echo "Check for any unexpected behavior"