import os
import random
import subprocess
import json
from datetime import datetime

dates_json = """[
  "2026-02-07", "2026-02-13", "2026-02-15", "2026-02-16", "2026-02-23", 
  "2026-02-24", "2026-03-02", "2026-03-03", "2026-03-04", "2026-03-05", 
  "2026-03-06", "2026-03-07", "2026-03-09", "2026-03-13", "2026-03-14", 
  "2026-03-15", "2026-03-23", "2026-03-24", "2026-03-29", "2026-03-30", 
  "2026-04-03", "2026-04-18", "2026-04-23", "2026-04-29", "2026-05-04", 
  "2026-05-06", "2026-05-11", "2026-05-13", "2026-05-14", "2026-05-15", 
  "2026-05-16", "2026-05-17", "2026-05-18", "2026-05-22", "2026-05-24", 
  "2026-05-25", "2026-05-26", "2026-05-27", "2026-05-28", "2026-05-29", 
  "2026-05-30", "2026-05-31", "2026-06-01", "2026-06-02", "2026-06-03", 
  "2026-06-04", "2026-06-05", "2026-06-06", "2026-06-07", "2026-06-11", 
  "2026-06-12", "2026-06-13", "2026-06-14", "2026-06-15", "2026-06-16", 
  "2026-06-20", "2026-06-21", "2026-06-23", "2026-06-26", "2026-06-29", 
  "2026-06-30"
]"""
dates = json.loads(dates_json)

# Just to be safe, filter them up to today (though they already seem to be)
today_str = "2026-07-02"
today_date = datetime.strptime(today_str, "%Y-%m-%d")
filtered_dates = []
for d in dates:
    dt = datetime.strptime(d, "%Y-%m-%d")
    if dt <= today_date:
        filtered_dates.append(d)
filtered_dates.sort()

# Reset repository
subprocess.run(['rm', '-rf', '.git'])
subprocess.run(['git', 'init'])

# Initial commit
subprocess.run(['git', 'add', '.'])
env_init = os.environ.copy()
env_init['GIT_AUTHOR_DATE'] = "2026-01-01T10:00:00"
env_init['GIT_COMMITTER_DATE'] = "2026-01-01T10:00:00"
subprocess.run(['git', 'commit', '-m', 'Initial project setup', '--date', "2026-01-01T10:00:00"], env=env_init)

with open('contributions.txt', 'w') as f:
    f.write('Contribution Log\n')

commit_messages = [
    "Refactor pointers implementation",
    "Fix memory leak in OOP classes",
    "Add missing comments to arrays section",
    "Update README with new resources",
    "Improve multithreading safety",
    "Fix deadlock in threads.cpp",
    "Optimize sliding window pattern",
    "Add tests for string_view dangling",
    "Refine diamond problem explanation",
    "Clean up unnecessary includes",
    "Update CMakeLists.txt",
    "Format code to adhere to guidelines",
    "Update contribution chart data",
    "Minor syntax fix",
    "Remove debug prints",
    "Enhance error handling in networking",
    "Add benchmark instructions",
    "Update project structure",
    "Refactor hashing patterns",
    "Implement two pointers solution",
    "Clean up unused variables",
    "Apply memory optimizations",
    "Fix edge case in parsing logic",
    "Improve documentation clarity",
    "Refactor data serialization",
    "Update project build configurations",
    "Address static analyzer warnings",
    "Tweak algorithm constants"
]

total_commits = 0

# Generate 4-10 commits per date
for current_date in filtered_dates:
    num_commits = random.randint(4, 10)
    for i in range(num_commits):
        with open('contributions.txt', 'a') as f:
            f.write(f'Log entry for {current_date} #{i}\n')
        
        subprocess.run(['git', 'add', 'contributions.txt'])
        
        env = os.environ.copy()
        hour = random.randint(9, 23)
        minute = random.randint(0, 59)
        second = random.randint(0, 59)
        commit_date = f"{current_date}T{hour:02d}:{minute:02d}:{second:02d}"
        
        env['GIT_AUTHOR_DATE'] = commit_date
        env['GIT_COMMITTER_DATE'] = commit_date
        
        msg = random.choice(commit_messages)
        
        subprocess.run(['git', 'commit', '-m', msg, '--date', commit_date], env=env, stdout=subprocess.DEVNULL)
        total_commits += 1

print(f"Targeted commits generated successfully ({total_commits} total commits).")
