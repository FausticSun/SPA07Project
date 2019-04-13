Invoke-Expression -Command ".\autotester.exe affects_source.txt affects_queries.txt affects.xml"
Invoke-Expression -Command ".\autotester.exe calls_source.txt calls_queries.txt calls.xml"
Invoke-Expression -Command ".\autotester.exe stress_source.txt stress_queries.txt stress.xml"
Invoke-Expression -Command ".\autotester.exe .\test1\test1_source.txt .\test1\test1_queries.txt .\test1\test1.xml"
