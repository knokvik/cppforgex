import os
import random
from datetime import date, timedelta
import subprocess

start_date = date(2026, 1, 1)
end_date = date(2026, 7, 2)

# Ensure git is initialized
if not os.path.exists('.git'):
    subprocess.run(['git', 'init'])

# Initial commit of everything
subprocess.run(['git', 'add', '.'])
env_init = os.environ.copy()
init_date = f"{start_date.isoformat()}T10:00:00"
env_init['GIT_AUTHOR_DATE'] = init_date
env_init['GIT_COMMITTER_DATE'] = init_date
subprocess.run(['git', 'commit', '-m', 'Initial commit of the project', '--date', init_date], env=env_init)

current_date = start_date
delta = timedelta(days=1)

with open('contributions.txt', 'w') as f:
    f.write('Contribution Log\n')

while current_date <= end_date:
    num_commits = random.randint(3, 6)
    for i in range(num_commits):
        with open('contributions.txt', 'a') as f:
            f.write(f'Commit on {current_date.isoformat()} #{i}\n')
        
        subprocess.run(['git', 'add', 'contributions.txt'])
        
        # Set environment variables for the commit date
        env = os.environ.copy()
        hour = random.randint(9, 23)
        minute = random.randint(0, 59)
        second = random.randint(0, 59)
        commit_date = f"{current_date.isoformat()}T{hour:02d}:{minute:02d}:{second:02d}"
        
        env['GIT_AUTHOR_DATE'] = commit_date
        env['GIT_COMMITTER_DATE'] = commit_date
        
        subprocess.run(['git', 'commit', '-m', f'Update contributions for {current_date.isoformat()}', '--date', commit_date], env=env, stdout=subprocess.DEVNULL)
        
    current_date += delta

print("Commits generated successfully.")
