import bashlex

parts = bashlex.split("""ls
ls""")
print list(parts)