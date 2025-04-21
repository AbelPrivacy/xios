const path = require("path");
const client = require("./build/Release/secure_http_client");

client.initialize(
  path.resolve(__dirname, "metadata.db"),
  ["TLSv1.3"],
  ["TLS13-AES256-GCM-SHA384"],
  [],
  [], // or ['KYBER_LEVEL1'] if supported
);

try {
  const res = client.post("https://localhost:1443/request/", "Hello World!");
  console.log(res);
} catch (e) {
  console.error("Request failed:", e.message);
}
