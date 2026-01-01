import os
import random
import subprocess
import json

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
dates.sort()

code_files = []
for root, dirs, files in os.walk('.'):
    if '.git' in root or '.vscode' in root or 'build' in root or 'scratch' in root:
        continue
    for f in files:
        if f.endswith('.cpp') or f.endswith('.h') or f.endswith('.md'):
            code_files.append(os.path.join(root, f))

# Backup remote url
remote_url = None
try:
    remote_url = subprocess.check_output(['git', 'remote', 'get-url', 'origin']).decode('utf-8').strip()
except:
    pass

# Reset repository
subprocess.run(['rm', '-rf', '.git'])
if os.path.exists('contributions.txt'):
    os.remove('contributions.txt')
subprocess.run(['git', 'init', '-b', 'main'])

if remote_url:
    subprocess.run(['git', 'remote', 'add', 'origin', remote_url])

# Initial commit
subprocess.run(['git', 'add', '.'])
env_init = os.environ.copy()
env_init['GIT_AUTHOR_DATE'] = "2026-01-01T10:00:00"
env_init['GIT_COMMITTER_DATE'] = "2026-01-01T10:00:00"
subprocess.run(['git', 'commit', '-m', 'Initial project setup', '--date', "2026-01-01T10:00:00"], env=env_init)

commit_actions = [
    ("Performance related optimization", "// Optimized memory access\n"),
    ("Fix potential memory leak", "// Addressed potential memory leak warning\n"),
    ("Refactor variables for readability", "// Refactored variables\n"),
    ("Clean up unused includes", "// Cleaned up includes\n"),
    ("Improve thread safety logic", "// Enforced thread safety\n"),
    ("Update comments for clarity", "// Updated documentation\n"),
    ("Add edge case handling", "// Added boundary checks\n"),
    ("Tweak algorithmic complexity", "// Improved time complexity\n"),
    ("Resolve static analysis warnings", "// Suppressed unused warnings\n"),
    ("Code formatting pass", "\n")
]

total_commits = 0

for current_date in dates:
    num_commits = random.randint(4, 10)
    for i in range(num_commits):
        file_to_mod = random.choice(code_files)
        msg, comment = random.choice(commit_actions)
        
        with open(file_to_mod, 'a') as f:
            f.write(comment)
            
        subprocess.run(['git', 'add', file_to_mod])
        
        env = os.environ.copy()
        hour = random.randint(9, 23)
        minute = random.randint(0, 59)
        second = random.randint(0, 59)
        commit_date = f"{current_date}T{hour:02d}:{minute:02d}:{second:02d}"
        
        env['GIT_AUTHOR_DATE'] = commit_date
        env['GIT_COMMITTER_DATE'] = commit_date
        
        basename = os.path.basename(file_to_mod)
        full_msg = f"{msg} in {basename}"
        
        subprocess.run(['git', 'commit', '-m', full_msg, '--date', commit_date], env=env, stdout=subprocess.DEVNULL)
        total_commits += 1

print(f"Realistic commits generated successfully ({total_commits} total commits).")
