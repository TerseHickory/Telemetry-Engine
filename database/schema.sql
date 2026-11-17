CREATE TABLE samples (
    timestamp_ns INTEGER NOT NULL CHECK(timestamp_ns > 0),
    source_id INTEGER NOT NULL CHECK(source_id >= 0),
    metric_id INTEGER NOT NULL CHECK(metric_id >= 0),
    metric_value REAL NOT NULL,
    CONSTRAINT FK_source_id FOREIGN KEY (source_id)
    REFERENCES sources (source_id)
    ON DELETE RESTRICT
);

CREATE TABLE sources (
    source_id INTEGER PRIMARY KEY,
    source_name TEXT NOT NULL UNIQUE,
    source_type_id INTEGER NOT NULL CHECK(0 < source_type_id < 4)
    CONSTRAINT FK_source_type_id FOREIGN KEY (source_type_id)
    REFERENCES source_types (source_type_id)
    ON DELETE RESTRICT
);

CREATE TABLE source_types (
    source_type_id INTEGER PRIMARY KEY,
    source_type_name TEXT NOT NULL UNIQUE CHECK(source_type_name = "memory" OR source_type_name = "cpu" OR source_type_name = "process")
);

CREATE TABLE metrics (
    metric_id INTEGER PRIMARY KEY,
    metric_name TEXT NOT NULL UNIQUE
);

