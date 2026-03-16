<script>
  import { onDestroy, onMount } from 'svelte';
  import { createMotorClient } from './lib/mqttMotorClient';

  let motorA = 0;
  let motorB = 0;
  let status = 'Connecting…';
  let client;

  function updateMotor(motorId, speed) {
    if (!client) {
      return;
    }

    client.publishMotorSpeed(motorId, speed);
  }

  onMount(() => {
    try {
      client = createMotorClient();
      client.on('connect', () => {
        status = 'Connected to broker';
      });

      client.on('reconnect', () => {
        status = 'Reconnecting…';
      });

      client.on('error', (err) => {
        status = `MQTT error: ${err.message}`;
      });
    } catch (err) {
      status = err.message;
    }
  });

  onDestroy(() => {
    client?.disconnect();
  });
</script>

<main class="min-h-screen bg-gradient-to-br from-black via-violet-950 to-black px-6 py-10">
  <section class="mx-auto max-w-3xl rounded-2xl border border-violet-400/40 bg-black/50 p-6 shadow-2xl shadow-violet-900/50">
    <header class="mb-8 flex items-center justify-between">
      <div>
        <p class="text-sm uppercase tracking-[0.2em] text-violet-300">Bygg 2.0</p>
        <h1 class="text-3xl font-bold text-violet-100">Dual Motor Control</h1>
      </div>
      <span class="rounded-full bg-violet-500/20 px-4 py-2 text-sm text-violet-200">{status}</span>
    </header>

    <div class="grid gap-6 md:grid-cols-2">
      <article class="rounded-xl border border-violet-500/30 bg-violet-950/30 p-4">
        <h2 class="mb-3 text-lg font-semibold text-violet-100">Motor A</h2>
        <input
          class="w-full accent-violet-400"
          type="range"
          min="-255"
          max="255"
          bind:value={motorA}
          on:input={() => updateMotor('motorA', motorA)}
        />
        <p class="mt-3 text-sm text-violet-200">Speed: {motorA}</p>
      </article>

      <article class="rounded-xl border border-violet-500/30 bg-violet-950/30 p-4">
        <h2 class="mb-3 text-lg font-semibold text-violet-100">Motor B</h2>
        <input
          class="w-full accent-violet-400"
          type="range"
          min="-255"
          max="255"
          bind:value={motorB}
          on:input={() => updateMotor('motorB', motorB)}
        />
        <p class="mt-3 text-sm text-violet-200">Speed: {motorB}</p>
      </article>
    </div>
  </section>
</main>
