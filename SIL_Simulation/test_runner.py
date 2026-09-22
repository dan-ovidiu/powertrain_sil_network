import subprocess
import csv
import io
import matplotlib
matplotlib.use('Agg') # Must be called BEFORE importing pyplot
import matplotlib.pyplot as plt

def run_network_simulation(throttle_cmd):
    print(f"\nExecuting SIL Network Test (Throttle: {throttle_cmd}%)...")
    
    import os
    import sys
    exe_name = 'network_sil.exe' if sys.platform == 'win32' else 'network_sil'
    
    if os.path.exists(os.path.join('.', exe_name)):
        exe_path = os.path.join('.', exe_name)
    else:
        exe_path = os.path.join('..', exe_name)
        
    # Pass the throttle command as an argument to the C executable!
    process = subprocess.run([exe_path, str(throttle_cmd)], capture_output=True, text=True)
    
    if process.returncode != 0:
        print("Simulation crashed!")
        return None
        
    reader = csv.DictReader(io.StringIO(process.stdout.strip()))
    
    steps, throttles, gears, rpms, speeds = [], [], [], [], []
    for row in reader:
        try:
            steps.append(int(row['Step']))
            throttles.append(float(row['Throttle']))
            gears.append(int(row['Gear']))
            rpms.append(float(row['RPM']))
            speeds.append(float(row['Speed']))
        except ValueError:
            pass
            
    return steps, throttles, gears, rpms, speeds

def generate_report(steps, throttles, gears, rpms, speeds, throttle_cmd):
    fig, (ax1, ax2, ax3, ax4) = plt.subplots(4, 1, figsize=(10, 10), sharex=True)
    
    ax1.plot(steps, throttles, 'b-', linewidth=2)
    ax1.set_ylabel('Throttle (%)')
    ax1.set_title(f'Automated Powertrain Network Test ({throttle_cmd}% Throttle)')
    ax1.grid(True)
    
    ax2.step(steps, gears, 'g-', linewidth=2, where='post')
    ax2.set_ylabel('Gear')
    ax2.set_yticks([1, 2, 3])
    ax2.grid(True)
    
    ax3.plot(steps, rpms, 'r-', linewidth=2)
    ax3.set_ylabel('Engine RPM')
    ax3.grid(True)
    
    ax4.plot(steps, speeds, 'm-', linewidth=2)
    ax4.set_ylabel('Speed (km/h)')
    ax4.set_xlabel('Simulation Step (10ms)')
    ax4.grid(True)
    
    plt.tight_layout()
    filename = f'sil_test_report_{throttle_cmd}.png'
    plt.savefig(filename)
    print(f"Plot saved to {filename}")

def main():
    print("=== STARTING AUTOMATED CI/CD TEST SUITE ===")
    
    # ---------------------------------------------------------
    # TEST 1: DRAG RACE (100% Throttle)
    # ---------------------------------------------------------
    steps, th, gear, rpm, speed = run_network_simulation(100)
    assert len(steps) == 100, "Simulation did not complete 100 steps."
    assert gear[-1] == 3, f"Drag race failed! Expected final gear 3, got {gear[-1]}"
    assert max(speed) > 75, f"Drag race failed! Expected max speed > 75 km/h, got {max(speed):.1f}"
    generate_report(steps, th, gear, rpm, speed, 100)
    print("Test 1 (Drag Race) Passed! [OK]")
    
    # ---------------------------------------------------------
    # TEST 2: CITY DRIVING (30% Throttle)
    # ---------------------------------------------------------
    steps, th, gear, rpm, speed = run_network_simulation(30)
    assert max(speed) < 50, f"City test failed! Speed exceeded 50 km/h (Max: {max(speed):.1f})"
    generate_report(steps, th, gear, rpm, speed, 30)
    print("Test 2 (City Driving) Passed! [OK]")

    # ---------------------------------------------------------
    # TEST 3: HIGHWAY CRUISING (50% Throttle)
    # ---------------------------------------------------------
    steps, th, gear, rpm, speed = run_network_simulation(50)
    assert 45 <= max(speed) <= 90, f"Highway test failed! Max speed {max(speed):.1f} out of 45-90 bounds."
    generate_report(steps, th, gear, rpm, speed, 50)
    print("Test 3 (Highway Cruising) Passed! [OK]")
    
    print("\n=== ALL CI/CD TESTS PASSED SUCESSFULLY ===")

if __name__ == "__main__":
    main()
