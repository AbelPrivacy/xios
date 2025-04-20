const path = require('path');
const client = require('./build/Release/secure_http_client');

client.initialize(
  path.resolve(__dirname, 'metadata.db'),
  ['TLSv1.3'],
  ['TLS13-AES256-GCM-SHA384'],
  [],
  [] // or ['KYBER_LEVEL1'] if supported
);

try {
  const res = client.get('https://localhost/request/');
  console.log(res);
} catch (e) {
  console.error('Request failed:', e.message);
}
