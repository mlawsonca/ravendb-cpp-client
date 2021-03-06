#pragma once
#include "AbstractDocumentQuery.h"
#include "IRawDocumentQuery.h"

namespace ravendb::client::documents::session
{
	template<typename T>
	class RawDocumentQuery : 
		public AbstractDocumentQuery<T>,
		public IRawDocumentQuery<T, RawDocumentQuery<T>>
	{
	private:
		RawDocumentQuery(std::shared_ptr<InMemoryDocumentSessionOperations> session, std::string raw_query);

	public:
		static std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> create(
			std::shared_ptr<InMemoryDocumentSessionOperations> session, std::string raw_query);

		std::shared_ptr<conventions::DocumentConventions> get_conventions() const;


		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> skip(int32_t count);

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> take(int32_t count);

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> wait_for_non_stale_results(
			const std::optional<std::chrono::milliseconds>& wait_timeout = {});

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> timings(
			std::optional<queries::timings::QueryTimings>& timings);

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> no_tracking();

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> no_caching();

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> using_default_operator(
			queries::QueryOperator query_operator);

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> statistics(std::shared_ptr<QueryStatistics>& stats) const;

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> wait_for_non_stale_results(
			const std::chrono::milliseconds& timeout = {});

		queries::IndexQuery get_index_query() const;

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> add_after_query_executed_listener(
			std::function<void(const queries::QueryResult&)> action);

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> remove_after_query_executed_listener(
			std::function<void(const queries::QueryResult&)> action);

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> add_before_query_executed_listener(
			std::function<void(const queries::IndexQuery&)> action);

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> remove_before_query_executed_listener(
			std::function<void(const queries::IndexQuery&)> action);

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> add_after_stream_executed_listener(
			std::function<void(const nlohmann::json&)> action);

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> remove_after_stream_executed_listener(
			std::function<void(const nlohmann::json&)> action);

		std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery>> add_parameter(std::string name, nlohmann::json value);

		Lazy<int32_t> count_lazily();

		std::shared_ptr<T> first(const std::optional<DocumentInfo::FromJsonConverter>& from_json);

		std::shared_ptr<T> first_or_default(const std::optional<DocumentInfo::FromJsonConverter>& from_json = {});

		std::shared_ptr<T> single(const std::optional<DocumentInfo::FromJsonConverter>& from_json = {});

		std::shared_ptr<T> single_or_default(const std::optional<DocumentInfo::FromJsonConverter>& from_json = {});

		bool any();

		int32_t count();

		Lazy<std::vector<std::shared_ptr<T>>> lazily(
			const std::optional<std::function<void(const std::vector<std::shared_ptr<T>>&)>>& on_eval = {});

		std::vector<std::shared_ptr<T>> to_list(const std::optional<DocumentInfo::FromJsonConverter>& from_json = {});
	};

	template <typename T>
	RawDocumentQuery<T>::RawDocumentQuery(std::shared_ptr<InMemoryDocumentSessionOperations> session,
		std::string raw_query)
		: AbstractDocumentQuery<T>(session, {}, {}, false, {}, {}, {})
	{
		AbstractDocumentQuery<T>::query_raw = std::move(raw_query);
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::create(
		std::shared_ptr<InMemoryDocumentSessionOperations> session, std::string raw_query)
	{
		auto object = std::shared_ptr<RawDocumentQuery>(new RawDocumentQuery(session, std::move(raw_query)));
		object->_weak_this = object;
		object->complete_construction();

		return object;
	}

	template <typename T>
	std::shared_ptr<conventions::DocumentConventions> RawDocumentQuery<T>::get_conventions() const
	{
		return AbstractDocumentQuery<T>::get_conventions();
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::skip(int32_t count)
	{
		AbstractDocumentQuery<T>::_skip(count);
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::take(int32_t count)
	{
		AbstractDocumentQuery<T>::_take(count);
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::wait_for_non_stale_results(
		const std::optional<std::chrono::milliseconds>& wait_timeout)
	{
		AbstractDocumentQuery<T>::_wait_for_non_stale_results(wait_timeout);
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::timings(
		std::optional<queries::timings::QueryTimings>& timings)
	{
		AbstractDocumentQuery<T>::_wait_for_non_stale_results(timings);
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::no_tracking()
	{
		AbstractDocumentQuery<T>::_no_tracking();
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::no_caching()
	{
		AbstractDocumentQuery<T>::_no_caching();
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::using_default_operator(
		queries::QueryOperator query_operator)
	{
		AbstractDocumentQuery<T>::_using_default_operator(query_operator);
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::statistics(
		std::shared_ptr<QueryStatistics>& stats) const
	{
		AbstractDocumentQuery<T>::_statistics(stats);
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::wait_for_non_stale_results(
		const std::chrono::milliseconds& timeout)
	{
		AbstractDocumentQuery<T>::_wait_for_non_stale_results(timeout);
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	queries::IndexQuery RawDocumentQuery<T>::get_index_query() const
	{
		return AbstractDocumentQuery<T>::get_index_query();
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::
		remove_after_query_executed_listener(std::function<void(const queries::QueryResult&)> action)
	{
		AbstractDocumentQuery<T>::_remove_after_query_executed_listener(action);
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::add_after_query_executed_listener(
		std::function<void(const queries::QueryResult&)> action)
	{
		AbstractDocumentQuery<T>::_add_after_query_executed_listener(action);
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::add_before_query_executed_listener(
		std::function<void(const queries::IndexQuery&)> action)
	{
		AbstractDocumentQuery<T>::_add_before_query_executed_listener(action);
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::
		remove_before_query_executed_listener(std::function<void(const queries::IndexQuery&)> action)
	{
		AbstractDocumentQuery<T>::_remove_before_query_executed_listener(action);
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::add_after_stream_executed_listener(
		std::function<void(const nlohmann::json&)> action)
	{
		AbstractDocumentQuery<T>::_add_after_stream_executed_listener(action);
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::
		remove_after_stream_executed_listener(std::function<void(const nlohmann::json&)> action)
	{
		AbstractDocumentQuery<T>::_remove_after_stream_executed_listener(action);
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	std::shared_ptr<IRawDocumentQuery<T, RawDocumentQuery<T>>> RawDocumentQuery<T>::add_parameter(
		std::string name, nlohmann::json value)
	{
		AbstractDocumentQuery<T>::_add_parameter(std::move(name), std::move(value));
		return std::static_pointer_cast<RawDocumentQuery<T>>(AbstractDocumentQuery<T>::_weak_this.lock());
	}

	template <typename T>
	Lazy<int32_t> RawDocumentQuery<T>::count_lazily()
	{
		return AbstractDocumentQuery<T>::count_lazily();
	}

	template <typename T>
	std::shared_ptr<T> RawDocumentQuery<T>::first(const std::optional<DocumentInfo::FromJsonConverter>& from_json)
	{
		return AbstractDocumentQuery<T>::first(from_json);
	}

	template <typename T>
	std::shared_ptr<T> RawDocumentQuery<T>::first_or_default(const std::optional<DocumentInfo::FromJsonConverter>& from_json)
	{
		return AbstractDocumentQuery<T>::first_or_default(from_json);
	}

	template <typename T>
	std::shared_ptr<T> RawDocumentQuery<T>::single(const std::optional<DocumentInfo::FromJsonConverter>& from_json)
	{
		return AbstractDocumentQuery<T>::single(from_json);
	}

	template <typename T>
	std::shared_ptr<T> RawDocumentQuery<T>::single_or_default(
		const std::optional<DocumentInfo::FromJsonConverter>& from_json)
	{
		return AbstractDocumentQuery<T>::single_or_default(from_json);
	}

	template <typename T>
	bool RawDocumentQuery<T>::any()
	{
		return AbstractDocumentQuery<T>::any();
	}

	template <typename T>
	int32_t RawDocumentQuery<T>::count()
	{
		return AbstractDocumentQuery<T>::count();
	}

	template <typename T>
	Lazy<std::vector<std::shared_ptr<T>>> RawDocumentQuery<T>::lazily(
		const std::optional<std::function<void(const std::vector<std::shared_ptr<T>>&)>>& on_eval)
	{
		return AbstractDocumentQuery<T>::lazily(on_eval);
	}

	template <typename T>
	std::vector<std::shared_ptr<T>> RawDocumentQuery<T>::to_list(const std::optional<DocumentInfo::FromJsonConverter>& from_json)
	{
		return AbstractDocumentQuery<T>::to_list(from_json);
	}
}
